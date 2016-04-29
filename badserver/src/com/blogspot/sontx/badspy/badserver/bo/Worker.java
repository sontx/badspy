package com.blogspot.sontx.badspy.badserver.bo;

import com.blogspot.sontx.badspy.badserver.bean.SpyDataHeader;

import java.io.Closeable;
import java.io.IOException;
import java.net.Socket;

/**
 * Copyright 2016 by sontx
 * Created by sontx on 25/4/2016.
 */
public abstract class Worker extends Thread implements Closeable {
    private Socket socket;
    protected final SpyReader reader;
    protected final SpyWriter writer;
    private OnCompletedListener mOnCompletedListener = null;

    public void setOnCompletedListener(OnCompletedListener listener) {
        mOnCompletedListener = listener;
    }

    public Worker(Socket socket) throws IOException {
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

    protected abstract void processHeader(SpyDataHeader header) throws IOException;

    @Override
    public void close() throws IOException {
        socket.close();
    }

    public interface OnCompletedListener {
        void onCompleted(Worker worker);
    }
}

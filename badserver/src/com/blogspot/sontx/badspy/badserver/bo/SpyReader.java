package com.blogspot.sontx.badspy.badserver.bo;

import com.blogspot.sontx.badspy.badserver.Config;
import com.blogspot.sontx.badspy.badserver.bean.SpyDataHeader;
import com.blogspot.sontx.jlog.Log;

import java.io.IOException;
import java.io.InputStream;

/**
 * Copyright 2016 by sontx
 * Created by sontx on 20/4/2016.
 */
public class SpyReader {
    private InputStream in;

    public SpyReader(InputStream in) {
        this.in = in;
    }

    private boolean waitForAvailable(int timeout) throws IOException {
        long startMillis = System.currentTimeMillis();
        while ((in.available() == 0) && (System.currentTimeMillis() - startMillis < timeout)) {
            try {
                Thread.sleep(50);
            } catch (InterruptedException e) {
                break;
            }
        }
        return in.available() > 0;
    }

    public int read(byte[] buffer, int offset, int length, int timeout) throws IOException {
        int receivedBytes = 0;
        while (receivedBytes < length && waitForAvailable(timeout)) {
            receivedBytes += in.read(buffer, offset + receivedBytes, length - receivedBytes);
        }
        return receivedBytes;
    }

    public int read(byte[] buffer, int offset, int length) throws IOException {
        Log.d(String.format("Read %d bytes...", length));
        return read(buffer, offset, length, Config.READ_TIMEOUT);
    }

    public int read(byte[] buffer) throws IOException {
        return read(buffer, 0, buffer.length);
    }

    public SpyDataHeader readHeader() throws IOException {
        byte[] buffer = new byte[5];
        return read(buffer) == buffer.length ? SpyDataHeader.parse(buffer) : null;
    }

    public int readFlag() throws IOException {
        return waitForAvailable(Config.READ_TIMEOUT) ? in.read() : -1;
    }
}

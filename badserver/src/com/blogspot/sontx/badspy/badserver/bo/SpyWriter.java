package com.blogspot.sontx.badspy.badserver.bo;

import com.blogspot.sontx.badspy.badserver.Convert;
import com.blogspot.sontx.badspy.badserver.bean.SpyDataHeader;

import java.io.IOException;
import java.io.OutputStream;

/**
 * Copyright 2016 by sontx
 * Created by sontx on 20/4/2016.
 */
public class SpyWriter {
    private OutputStream out;

    public SpyWriter(OutputStream out) {
        this.out = out;
    }

    public void write(byte resp) throws IOException {
        out.write(resp);
    }

    public void write(SpyDataHeader header) throws IOException {
        byte[] buffer = new byte[5];
        buffer[0] = header.getContentType();
        System.arraycopy(Convert.integerToBytes(header.getContentLength()), 0, buffer, 1, 4);
        out.write(buffer);
    }

    public void write(byte[] buffer, int offset, int length) throws IOException {
        out.write(buffer, offset, length);
    }
}

package com.blogspot.sontx.badspy.badserver;

import com.blogspot.sontx.badspy.badserver.bean.SpyDateTime;

import java.util.Date;

/**
 * Copyright 2016 by sontx
 * Created by sontx on 20/4/2016.
 */
public final class Convert {

    public static int bytesToInteger(byte[] bytes, int offset) {
        return ((bytes[offset + 0] << 0) & 0x000000ff) |
               ((bytes[offset + 1] << 8) & 0x0000ff00) |
               ((bytes[offset + 2] << 16) & 0x00ff0000) |
               ((bytes[offset + 3] << 24) & 0xff000000);
    }

    public static byte[] integerToBytes(int i) {
        return new byte[] {
                (byte) ((i >> 0) & 0x000000ff),
                (byte) ((i >> 8) & 0x000000ff),
                (byte) ((i >> 16) & 0x000000ff),
                (byte) ((i >> 24) & 0x000000ff)};
    }

    public static SpyDateTime integerToDateTime(int seconds) {
        SpyDateTime dateTime = new SpyDateTime();
        dateTime.setHour((byte) ((seconds >> 0) & 0x1f));
        dateTime.setMinute((byte) ((seconds >> 5) & 0x3f));
        dateTime.setSecond((byte) ((seconds >> 11) & 0x3f));
        dateTime.setDay((byte) ((seconds >> 17) & 0x1f));
        dateTime.setMonth((byte) ((seconds >> 22) & 0x0f));
        dateTime.setYear((short) (2000 + ((seconds >> 26) & 0x3f)));
        return dateTime;
    }

    private Convert() {}
}

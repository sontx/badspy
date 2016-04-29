package com.blogspot.sontx.badspy.badserver.bo;

import com.blogspot.sontx.jlog.Log;

import java.io.Closeable;
import java.io.IOException;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.LinkedList;
import java.util.List;

/**
 * Copyright 2016 by sontx
 * Created by sontx on 25/4/2016.
 */
public abstract class Server<T extends Worker> extends Thread implements Closeable, Worker.OnCompletedListener {
    private ServerSocket serverSocket;
    private List<T> workers = new LinkedList<>();

    public Server(String addr, int port) throws IOException {
        Log.i(String.format("Server starting at %s:%d...", addr, port));
        serverSocket = new ServerSocket(port, 100, InetAddress.getByName(addr));
    }

    private void onAcceptSocket(Socket socket) throws IOException {
        T worker = createWorker(socket);
        if (worker != null) {
            startWorker(worker);
            Log.i("Worker is running...");
        }
    }

    private void startWorker(T worker) throws IOException {
        synchronized (this) {
            workers.add(worker);
            worker.setOnCompletedListener(this);
            worker.start();
        }
    }

    protected abstract T createWorker(Socket socket) throws IOException;

    private void waitForConnection() throws IOException {
        Log.i("Waiting for connection...");
        while (true) {
            Socket socket = serverSocket.accept();
            Log.i(String.format("Accepted connection at %s", socket.getRemoteSocketAddress()));
            onAcceptSocket(socket);
        }
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
            for (Worker worker : workers) {
                worker.close();
            }
            workers.clear();
        }
        Log.w("Closed server");
    }

    @Override
    public void onCompleted(Worker worker) {
        synchronized (this) {
            workers.remove(worker);
            worker.setOnCompletedListener(null);
            Log.w("Removed worker");
        }
    }
}

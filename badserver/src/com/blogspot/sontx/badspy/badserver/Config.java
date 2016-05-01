package com.blogspot.sontx.badspy.badserver;

import com.blogspot.sontx.jini.INIBadFormatException;
import com.blogspot.sontx.jini.INIFile;
import com.blogspot.sontx.jlog.Log;

import java.io.File;
import java.io.IOException;
import java.nio.charset.Charset;

/**
 * Copyright 2016 by sontx
 * Created by xuans on 20/4/2016.
 */
public final class Config {
    private static final String CONFIG_FILE_NAME = "config";
    public static final String WORKING_DIR;
    public static final String LOGGING_DIR;
    public static final String PACKAGE_DIR;
    public static final String DESC_FILENAME;
    public static final int READ_TIMEOUT;
    public static final int READ_FILE_BUFFER_SIZE;
    public static final String STORAGE_MAGIC_KBFILE;
    public static final String STORAGE_MAGIC_SCROTFILE;
    public static final String STORAGE_PREFIX_KBFILE ;
    public static final String STORAGE_PREFIX_SCROTFILE;
    public static final int SERVER_SPY_PORT;
    public static final int SERVER_BACKDOOR_PORT;
    public static final byte RESPONSE_OK = 1;
    public static final byte RESPONSE_CANCEL = 0;

    private Config() {}

    static {
        INIFile file = null;
        try {
            file = new INIFile(CONFIG_FILE_NAME, Charset.defaultCharset());
        } catch (IOException | INIBadFormatException e) {
            e.printStackTrace();
        }

        String working_dir = "";
        String desc_filename = "";
        int read_timeout = 0;
        int read_file_buffer_size = 0;
        String storage_magic_kbfile = "";
        String storage_magic_scrotfile = "";
        String storage_prefix_kbfile = "";
        String storage_prefix_scrotfile = "";
        int server_spy_port = 0;
        int server_backdoor_port = 0;

        try {
            working_dir = file.get("working-dir", System.getProperty("user.dir"), true);
            desc_filename = file.get("desc-name", "desc", true);
            read_timeout = Integer.parseInt(file.get("read-timeout", "20000", true));
            read_file_buffer_size = Integer.parseInt(file.get("read-buffer-size", "2048", true));
            storage_magic_kbfile = file.get("magic-kbfile", "KB", true);
            storage_magic_scrotfile = file.get("magic-scrotfile", "SS", true);
            storage_prefix_kbfile = file.get("prefix-kbfile", "klog", true);
            storage_prefix_scrotfile = file.get("prefix-scrotfile", "scrot", true);
            server_spy_port = Integer.parseInt(file.get("server-spy-port", "2512", true));
            server_backdoor_port = Integer.parseInt(file.get("server-backdoor-port", "3393", true));
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            WORKING_DIR = working_dir;
            DESC_FILENAME = desc_filename;
            READ_TIMEOUT = read_timeout;
            READ_FILE_BUFFER_SIZE = read_file_buffer_size;
            STORAGE_MAGIC_KBFILE = storage_magic_kbfile;
            STORAGE_MAGIC_SCROTFILE = storage_magic_scrotfile;
            STORAGE_PREFIX_KBFILE = storage_prefix_kbfile;
            STORAGE_PREFIX_SCROTFILE = storage_prefix_scrotfile;
            SERVER_SPY_PORT = server_spy_port;
            SERVER_BACKDOOR_PORT = server_backdoor_port;
        }

        LOGGING_DIR = new File(WORKING_DIR, "log").getAbsolutePath();
        PACKAGE_DIR = new File(WORKING_DIR, "pck").getAbsolutePath();

        Log.i(String.format("Loaded config file: %s", file.getBaseFile().getAbsolutePath()));
    }
}

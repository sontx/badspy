package com.blogspot.sontx.badspy.badserver;

/**
 * Copyright 2016 by sontx
 * Created by xuans on 20/4/2016.
 */
public final class Config {
    public static final String WORKING_DIR = "C:\\Users\\xuans\\Desktop\\spy\\server";
    public static final String DSC_FILENAME = "dsc";
    public static final int READ_TIMEOUT = 10000;
    public static final int READ_FILE_BUFFER_SIZE = 1024;
    public static final String STORAGE_MAGIC_KBFILE = "KB";
    public static final String STORAGE_MAGIC_SCROTFILE = "SS";
    public static final String STORAGE_PREFIX_KBFILE = "klog";
    public static final String STORAGE_PREFIX_SCROTFILE = "scrot";
    public static final byte RESPONSE_OK = 1;
    public static final byte RESPONSE_CANCEL = 0;

    private Config() {}
}

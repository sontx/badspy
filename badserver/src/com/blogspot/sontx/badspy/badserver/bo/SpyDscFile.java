package com.blogspot.sontx.badspy.badserver.bo;

import com.blogspot.sontx.jini.INIBadFormatException;
import com.blogspot.sontx.jini.INIFile;

import java.io.File;
import java.io.IOException;
import java.nio.charset.Charset;

/**
 * Copyright 2016 by sontx
 * Created by sontx on 21/4/2016.
 */
public class SpyDscFile extends INIFile {
    public static final String KEY_HOSTNAME = "hostname";
    private static final Charset DEF_CHARSET = Charset.forName("ASCII");

    public SpyDscFile(String filePath) throws IOException, INIBadFormatException {
        super(filePath, DEF_CHARSET);
    }

    public String put(String key, byte[] stringBytes) throws IOException {
        String value = new String(stringBytes, DEF_CHARSET);
        super.put(key, value);
        return value;
    }
}

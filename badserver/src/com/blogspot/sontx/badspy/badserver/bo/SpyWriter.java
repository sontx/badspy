package com.blogspot.sontx.badspy.badserver.bo;

import java.io.IOException;
import java.io.OutputStream;

/**
 * Copyright 2016 by sontx
 * Created by sontx on 20/4/2016.
 */
public class SpyWriter {
    private OutputStream out;

    public SpyWriter(OutputStream out) {
        this.out = out;
    }

    public void write(byte resp) throws IOException {
        out.write(resp);
    }
}

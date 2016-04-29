package com.blogspot.sontx.badspy.badserver.bo;

import com.blogspot.sontx.badspy.badserver.bo.SpyReader;
import com.blogspot.sontx.badspy.badserver.bo.SpyWriter;

import java.io.IOException;

/**
 * Copyright 2016 by sontx
 * Created by sontx on 20/4/2016.
 */
public abstract class Receiver {
    protected final SpyReader reader;
    protected final SpyWriter writer;
    protected final int dataLength;

    public Receiver(SpyReader reader, SpyWriter writer, int dataLength) {
        this.reader = reader;
        this.writer = writer;
        this.dataLength = dataLength;
    }

    public abstract void start() throws IOException;
}

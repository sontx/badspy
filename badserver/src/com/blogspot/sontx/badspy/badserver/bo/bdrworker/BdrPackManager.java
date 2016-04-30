package com.blogspot.sontx.badspy.badserver.bo.bdrworker;

import com.blogspot.sontx.badspy.badserver.bean.Pack;
import com.blogspot.sontx.badspy.badserver.bean.Version;

import javax.naming.InvalidNameException;
import java.io.File;
import java.util.ArrayList;
import java.util.List;

/**
 * Copyright 2016 by sontx
 * Created by sontx on 25/4/2016.
 */
public final class BdrPackManager {
    private BdrPackManager() {}

    private static File workingDir;

    public static void init(String _workingDir) {
        workingDir = new File(_workingDir);
        if (!workingDir.isDirectory())
            workingDir.mkdirs();
    }

    public static Pack getPackById(int id) {
        try {
            String packFileName = getPackFileNameById(id);
            return packFileName != null ? new Pack(packFileName) : null;
        } catch (InvalidNameException e) {
            e.printStackTrace();
        }
        return null;
    }

    public static File getPackFileById(int id) {
        String packFileName = getPackFileNameById(id);
        return packFileName == null ? null : new File(workingDir, packFileName);
    }

    private static String getPackFileNameById(int id) {
        String[] packFileNames = workingDir.list();
        if (packFileNames == null)
            return null;
        for (String packFileName : packFileNames) {
            if (Pack.getPackIdByFileName(packFileName) == id)
                return packFileName;
        }
        return null;
    }

    public static List<Pack> getAllPacks() {
        List<Pack> packs = new ArrayList<>();
        String[] packFileNames = workingDir.list();
        if (packFileNames == null)
            return packs;
        for (String packFileName : packFileNames) {
            try {
                packs.add(new Pack(packFileName));
            } catch (InvalidNameException e) {
                e.printStackTrace();
            }
        }
        return packs;
    }
}

package com.blogspot.sontx.badspy.badserver.bean;

import javax.naming.InvalidNameException;

/**
 * Copyright 2016 by sontx
 * Created by sontx on 25/4/2016.
 * Pack: abc{name}.x.y.z
 */
public class Pack {
    private int id;
    private String name;
    private Version version;

    public static int getPackIdByFileName(String fileName) {
        if (fileName == null || fileName.length() < 3)
            return -1;
        String sId = fileName.substring(0, 3);
        try {
            return Integer.parseInt(sId);
        } catch (NumberFormatException e){}
        return -1;
    }

    private Version getPackVersionByFileName(String fileName) {
        int versionOffset = fileName.indexOf(".");
        if (versionOffset == -1)
            return null;
        String versionString = fileName.substring(versionOffset + 1, fileName.lastIndexOf("."));
        return Version.parse(versionString);
    }

    private String getPackNameByFileName(String fileName) {
        if (fileName == null || fileName.length() <= 3)
            return null;
        int versionOffset = fileName.indexOf(".");
        return versionOffset <= 3 ? null : fileName.substring(3, versionOffset);
    }

    public Pack() {}

    public Pack(String packFileName) throws InvalidNameException {
        if ((id = getPackIdByFileName(packFileName)) == -1)
            throw new InvalidNameException("Pack id");
        if ((name = getPackNameByFileName(packFileName)) == null)
            throw new InvalidNameException("Pack name");
        if ((version = getPackVersionByFileName(packFileName)) == null)
            throw new InvalidNameException("Pack version");
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public Version getVersion() {
        return version;
    }

    public void setVersion(Version version) {
        this.version = version;
    }

    @Override
    public String toString() {
        return String.format("%03d%s.%s", id, name, version);
    }
}

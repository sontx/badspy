package com.blogspot.sontx.badspy.badserver.bean;

/**
 * Copyright 2016 by sontx
 * Created by sontx on 25/4/2016.
 */
public class Version {
    private byte major;
    private byte minor;
    private byte revision;

    public byte getMajor() {
        return major;
    }

    public void setMajor(byte major) {
        this.major = major;
    }

    public byte getMinor() {
        return minor;
    }

    public void setMinor(byte minor) {
        this.minor = minor;
    }

    public byte getRevision() {
        return revision;
    }

    public void setRevision(byte revision) {
        this.revision = revision;
    }

    public static Version parse(byte[] bytes) {
        if (bytes == null || bytes.length != 3)
            return null;
        Version version = new Version();
        version.major = bytes[0];
        version.minor = bytes[1];
        version.revision = bytes[2];
        return version;
    }

    public static Version parse(String st) {
        if (st == null)
            return null;
        String[] parts = st.split("\\.");
        if (parts.length != 3)
            return null;
        Version version = new Version();
        version.major = Byte.parseByte(parts[0]);
        version.minor = Byte.parseByte(parts[1]);
        version.revision = Byte.parseByte(parts[2]);
        return version;
    }

    @Override
    public String toString() {
        return String.format("%d.%d.%d", major, minor, revision);
    }

    public int compare(Version version) {
        if (major != version.major)
            return major - version.major;
        if (minor != version.minor)
            return minor - version.minor;
        return revision - version.revision;
    }
}

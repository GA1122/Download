    void writeVersion()
    {
        append(VersionTag);
        doWriteUint32(SerializedScriptValue::wireFormatVersion);
    }

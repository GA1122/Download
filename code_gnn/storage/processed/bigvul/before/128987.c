    bool readVersion(uint32_t& version)
    {
        SerializationTag tag;
        if (!readTag(&tag)) {
            version = 0;
            return true;
        }
        if (tag != VersionTag) {
            version = 0;
            undoReadTag();
            return true;
        }
        return doReadUint32(&version);
    }

    void writeFileIndex(int blobIndex)
    {
        append(FileIndexTag);
        doWriteUint32(blobIndex);
    }

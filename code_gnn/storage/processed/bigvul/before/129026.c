    void writeFileListIndex(const Vector<int>& blobIndices)
    {
        append(FileListIndexTag);
        uint32_t length = blobIndices.size();
        doWriteUint32(length);
        for (unsigned i = 0; i < length; ++i)
            doWriteUint32(blobIndices[i]);
    }

    void writeBlobIndex(int blobIndex)
    {
        ASSERT(blobIndex >= 0);
        append(BlobIndexTag);
        doWriteUint32(blobIndex);
    }

    void writeReferenceCount(uint32_t numberOfReferences)
    {
        append(ReferenceCountTag);
        doWriteUint32(numberOfReferences);
    }

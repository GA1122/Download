    void writeDenseArray(uint32_t numProperties, uint32_t length)
    {
        append(DenseArrayTag);
        doWriteUint32(numProperties);
        doWriteUint32(length);
    }

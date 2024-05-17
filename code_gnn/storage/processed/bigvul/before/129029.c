    void writeGenerateFreshSparseArray(uint32_t length)
    {
        append(GenerateFreshSparseArrayTag);
        doWriteUint32(length);
    }

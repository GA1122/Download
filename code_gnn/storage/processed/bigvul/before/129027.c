    void writeGenerateFreshDenseArray(uint32_t length)
    {
        append(GenerateFreshDenseArrayTag);
        doWriteUint32(length);
    }

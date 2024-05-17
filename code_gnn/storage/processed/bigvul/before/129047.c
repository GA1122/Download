    void writeTransferredArrayBuffer(uint32_t index)
    {
        append(ArrayBufferTransferTag);
        doWriteUint32(index);
    }

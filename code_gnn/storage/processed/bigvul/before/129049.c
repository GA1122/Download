    void writeTransferredMessagePort(uint32_t index)
    {
        append(MessagePortTag);
        doWriteUint32(index);
    }

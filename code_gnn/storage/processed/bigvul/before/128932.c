    void doWriteArrayBuffer(const ArrayBuffer& arrayBuffer)
    {
        uint32_t byteLength = arrayBuffer.byteLength();
        doWriteUint32(byteLength);
        append(static_cast<const uint8_t*>(arrayBuffer.data()), byteLength);
    }

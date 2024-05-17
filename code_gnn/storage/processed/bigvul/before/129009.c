    void writeArrayBuffer(const ArrayBuffer& arrayBuffer)
    {
        append(ArrayBufferTag);
        doWriteArrayBuffer(arrayBuffer);
    }

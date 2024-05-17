    StateBase* writeTransferredArrayBuffer(v8::Handle<v8::Value> value, uint32_t index, StateBase* next)
    {
        ArrayBuffer* arrayBuffer = V8ArrayBuffer::toNative(value.As<v8::Object>());
        if (!arrayBuffer)
            return 0;
        if (arrayBuffer->isNeutered())
            return handleError(DataCloneError, "An ArrayBuffer is neutered and could not be cloned.", next);
        m_writer.writeTransferredArrayBuffer(index);
        return 0;
    }

    StateBase* writeArrayBuffer(v8::Handle<v8::Value> value, StateBase* next)
    {
        ArrayBuffer* arrayBuffer = V8ArrayBuffer::toNative(value.As<v8::Object>());
        if (!arrayBuffer)
            return 0;
        if (arrayBuffer->isNeutered())
            return handleError(DataCloneError, "An ArrayBuffer is neutered and could not be cloned.", next);
        ASSERT(!m_transferredArrayBuffers.contains(value.As<v8::Object>()));
        m_writer.writeArrayBuffer(*arrayBuffer);
        return 0;
    }

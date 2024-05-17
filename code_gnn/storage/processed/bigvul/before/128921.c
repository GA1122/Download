    PassRefPtr<ArrayBuffer> doReadArrayBuffer()
    {
        uint32_t byteLength;
        if (!doReadUint32(&byteLength))
            return nullptr;
        if (m_position + byteLength > m_length)
            return nullptr;
        const void* bufferStart = m_buffer + m_position;
        RefPtr<ArrayBuffer> arrayBuffer = ArrayBuffer::create(bufferStart, byteLength);
        arrayBuffer->setDeallocationObserver(V8ArrayBufferDeallocationObserver::instanceTemplate());
        m_position += byteLength;
        return arrayBuffer.release();
    }

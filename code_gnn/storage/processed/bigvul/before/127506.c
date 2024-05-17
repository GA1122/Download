    void skipBytes(long numBytes)
    {
        decoder_source_mgr* src = (decoder_source_mgr*)m_info.src;
        long bytesToSkip = std::min(numBytes, (long)src->pub.bytes_in_buffer);
        src->pub.bytes_in_buffer -= (size_t)bytesToSkip;
        src->pub.next_input_byte += bytesToSkip;

        m_bytesToSkip = std::max(numBytes - bytesToSkip, static_cast<long>(0));
    }

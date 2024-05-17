  void WriteNextChunkToBuffer() {
    OffsetSizePair& chunk = shuffled_buf_.front();
    QuicStreamOffset offset = chunk.first;
    const size_t num_to_write = chunk.second;
    std::unique_ptr<char[]> write_buf{new char[max_chunk_size_bytes_]};
    for (size_t i = 0; i < num_to_write; ++i) {
      write_buf[i] = (offset + i) % 256;
    }
    QuicStringPiece string_piece_w(write_buf.get(), num_to_write);
    size_t written;
    auto result =
        buffer_->OnStreamData(offset, string_piece_w, clock_.ApproximateNow(),
                              &written, &error_details_);
    if (result == QUIC_NO_ERROR) {
      shuffled_buf_.pop_front();
      total_bytes_written_ += num_to_write;
    } else {
      shuffled_buf_.push_back(chunk);
      shuffled_buf_.pop_front();
    }
    QUIC_DVLOG(1) << " write at offset: " << offset
                  << " len to write: " << num_to_write
                  << " write result: " << result
                  << " left over: " << shuffled_buf_.size();
  }

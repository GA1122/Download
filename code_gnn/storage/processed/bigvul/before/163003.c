  void CreateSourceAndShuffle(size_t max_chunk_size_bytes) {
    max_chunk_size_bytes_ = max_chunk_size_bytes;
    std::unique_ptr<OffsetSizePair[]> chopped_stream(
        new OffsetSizePair[bytes_to_buffer_]);

    size_t start_chopping_offset = 0;
    size_t iterations = 0;
    while (start_chopping_offset < bytes_to_buffer_) {
      size_t max_chunk = std::min<size_t>(
          max_chunk_size_bytes_, bytes_to_buffer_ - start_chopping_offset);
      size_t chunk_size = rng_.RandUint64() % max_chunk + 1;
      chopped_stream[iterations] =
          OffsetSizePair(start_chopping_offset, chunk_size);
      start_chopping_offset += chunk_size;
      ++iterations;
    }
    DCHECK(start_chopping_offset == bytes_to_buffer_);
    size_t chunk_num = iterations;

    for (int i = chunk_num - 1; i >= 0; --i) {
      size_t random_idx = rng_.RandUint64() % (i + 1);
      QUIC_DVLOG(1) << "chunk offset " << chopped_stream[random_idx].first
                    << " size " << chopped_stream[random_idx].second;
      shuffled_buf_.push_front(chopped_stream[random_idx]);
      chopped_stream[random_idx] = chopped_stream[i];
    }
  }

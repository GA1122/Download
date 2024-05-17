  int64 ReadsSize(MockRead data_reads[], size_t reads_count) {
    int64 size = 0;
    for (size_t i = 0; i < reads_count; ++i)
      size += data_reads[i].data_len;
    return size;
  }

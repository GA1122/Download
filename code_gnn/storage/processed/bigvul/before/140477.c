  SimpleGetHelperResult SimpleGetHelper(MockRead data_reads[],
                                        size_t reads_count) {
    StaticSocketDataProvider reads(data_reads, reads_count, NULL, 0);
    StaticSocketDataProvider* data[] = { &reads };
    return SimpleGetHelperForData(data, 1);
  }

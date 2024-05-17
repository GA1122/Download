  TestMultiBufferDataProvider(UrlData* url_data, MultiBuffer::BlockId pos)
      : ResourceMultiBufferDataProvider(url_data,
                                        pos,
                                        false  ) {
    CHECK(test_data_providers.insert(this).second);
  }

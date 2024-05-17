  void SetupMockReader(
      bool valid_info, bool valid_data, bool valid_size) {
    net::HttpResponseInfo* info = valid_info ? MakeMockResponseInfo() : nullptr;
    int info_size = info ? GetResponseInfoSize(info) : 0;
    const char* data = valid_data ? kMockBody : nullptr;
    int data_size = valid_size ? kMockBodySize : 3;
    mock_storage()->SimulateResponseReader(
        new MockResponseReader(kMockResponseId, info, info_size,
                               data, data_size));
  }

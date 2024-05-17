  MockResponseReader(int64_t response_id,
                     net::HttpResponseInfo* info,
                     int info_size,
                     const char* data,
                     int data_size)
      : AppCacheResponseReader(response_id,  nullptr),
        info_(info),
        info_size_(info_size),
        data_(data),
        data_size_(data_size) {}

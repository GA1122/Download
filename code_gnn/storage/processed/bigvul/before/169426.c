ChunkedUploadDataStream::ChunkedUploadDataStream(int64_t identifier)
    : UploadDataStream(true, identifier),
      read_index_(0),
      read_offset_(0),
      all_data_appended_(false),
      read_buffer_len_(0),
      weak_factory_(this) {}

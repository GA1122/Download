  explicit ValidateDataUploadRequest(BinaryUploadService::Callback callback)
      : BinaryUploadService::Request(std::move(callback)) {}

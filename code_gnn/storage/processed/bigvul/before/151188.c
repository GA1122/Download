  InspectorFileReaderLoaderClient(
      RefPtr<BlobDataHandle> blob,
      const String& mime_type,
      const String& text_encoding_name,
      std::unique_ptr<GetResponseBodyCallback> callback)
      : blob_(std::move(blob)),
        mime_type_(mime_type),
        text_encoding_name_(text_encoding_name),
        callback_(std::move(callback)) {
    loader_ = FileReaderLoader::Create(FileReaderLoader::kReadByClient, this);
  }

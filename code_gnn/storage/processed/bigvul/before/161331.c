bool GetPostData(const net::URLRequest* request, std::string* post_data) {
  if (!request->has_upload())
    return false;

  const net::UploadDataStream* stream = request->get_upload();
  if (!stream->GetElementReaders())
    return false;

  const auto* element_readers = stream->GetElementReaders();

  if (element_readers->empty())
    return false;

  *post_data = "";
  for (const auto& element_reader : *element_readers) {
    const net::UploadBytesElementReader* reader =
        element_reader->AsBytesReader();
    if (!reader) {
      *post_data = "";
      return false;
    }
    *post_data += std::string(reader->bytes(), reader->length());
  }
  return true;
}

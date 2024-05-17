void AppendGzippedResource(const base::RefCountedMemory& encoded,
                           std::string* to_append) {
  std::unique_ptr<net::MockSourceStream> source_stream(
      new net::MockSourceStream());
  source_stream->AddReadResult(encoded.front_as<char>(), encoded.size(),
                               net::OK, net::MockSourceStream::SYNC);
  source_stream->AddReadResult(encoded.front_as<char>() + encoded.size(), 0,
                               net::OK, net::MockSourceStream::SYNC);
  std::unique_ptr<net::GzipSourceStream> filter = net::GzipSourceStream::Create(
      std::move(source_stream), net::SourceStream::TYPE_GZIP);
  scoped_refptr<net::IOBufferWithSize> dest_buffer =
      new net::IOBufferWithSize(4096);
  net::CompletionCallback callback;
  while (true) {
    int rv = filter->Read(dest_buffer.get(), dest_buffer->size(), callback);
    ASSERT_LE(0, rv);
    if (rv <= 0)
      break;
    to_append->append(dest_buffer->data(), rv);
  }
}

std::string ReadFromUploadDataStream(UploadDataStream* stream) {
  std::string data_read;
  scoped_refptr<IOBuffer> buf = new IOBuffer(kTestBufferSize);
  while (!stream->IsEOF()) {
    TestCompletionCallback callback;
    const int result =
        stream->Read(buf.get(), kTestBufferSize, callback.callback());
    const int bytes_read =
        result != ERR_IO_PENDING ? result : callback.WaitForResult();
    data_read.append(buf->data(), bytes_read);
  }
  return data_read;
}

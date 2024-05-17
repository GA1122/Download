int ResponseWriter::Write(net::IOBuffer* buffer,
                          int num_bytes,
                          const net::CompletionCallback& callback) {
  std::string chunk = std::string(buffer->data(), num_bytes);
  bool encoded = false;
  if (!base::IsStringUTF8(chunk)) {
    encoded = true;
    base::Base64Encode(chunk, &chunk);
  }

  base::Value* id = new base::Value(stream_id_);
  base::Value* chunkValue = new base::Value(chunk);
  base::Value* encodedValue = new base::Value(encoded);

  content::BrowserThread::PostTask(
      content::BrowserThread::UI, FROM_HERE,
      base::BindOnce(&DevToolsUIBindings::CallClientFunction, bindings_,
                     "DevToolsAPI.streamWrite", base::Owned(id),
                     base::Owned(chunkValue), base::Owned(encodedValue)));
  return num_bytes;
}

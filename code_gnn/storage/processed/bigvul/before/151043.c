int ResponseWriter::Write(net::IOBuffer* buffer,
                          int num_bytes,
                          const net::CompletionCallback& callback) {
  std::string chunk = std::string(buffer->data(), num_bytes);
  bool encoded = false;
  if (!base::IsStringUTF8(chunk)) {
    encoded = true;
    base::Base64Encode(chunk, &chunk);
  }

  base::FundamentalValue* id = new base::FundamentalValue(stream_id_);
  base::StringValue* chunkValue = new base::StringValue(chunk);
  base::FundamentalValue* encodedValue = new base::FundamentalValue(encoded);

  content::BrowserThread::PostTask(
      content::BrowserThread::UI, FROM_HERE,
      base::Bind(&DevToolsUIBindings::CallClientFunction, bindings_,
                 "DevToolsAPI.streamWrite", base::Owned(id),
                 base::Owned(chunkValue), base::Owned(encodedValue)));
  return num_bytes;
}

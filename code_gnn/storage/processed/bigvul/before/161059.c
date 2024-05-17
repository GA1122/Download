void DevToolsHttpHandler::DecompressAndSendJsonProtocol(int connection_id) {
  scoped_refptr<base::RefCountedMemory> raw_bytes =
      GetContentClient()->GetDataResourceBytes(COMPRESSED_PROTOCOL_JSON);
  const uint8_t* next_encoded_byte = raw_bytes->front();
  size_t input_size_remaining = raw_bytes->size();
  BrotliDecoderState* decoder = BrotliDecoderCreateInstance(
      nullptr  , nullptr  ,
      nullptr  );
  CHECK(!!decoder);
  std::vector<std::string> decoded_parts;
  size_t decompressed_size = 0;
  while (!BrotliDecoderIsFinished(decoder)) {
    size_t output_size_remaining = 0;
    CHECK(BrotliDecoderDecompressStream(
              decoder, &input_size_remaining, &next_encoded_byte,
              &output_size_remaining, nullptr,
              nullptr) != BROTLI_DECODER_RESULT_ERROR);
    const uint8_t* output_buffer =
        BrotliDecoderTakeOutput(decoder, &output_size_remaining);
    decoded_parts.emplace_back(reinterpret_cast<const char*>(output_buffer),
                               output_size_remaining);
    decompressed_size += output_size_remaining;
  }
  BrotliDecoderDestroyInstance(decoder);

  std::string json_protocol;
  json_protocol.reserve(decompressed_size);
  for (const std::string& part : decoded_parts) {
    json_protocol.append(part);
  }

  net::HttpServerResponseInfo response(net::HTTP_OK);
  response.SetBody(json_protocol, "application/json; charset=UTF-8");

  thread_->task_runner()->PostTask(
      FROM_HERE, base::BindOnce(&ServerWrapper::SendResponse,
                                base::Unretained(server_wrapper_.get()),
                                connection_id, response));
}

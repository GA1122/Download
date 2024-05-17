String FileReaderLoader::ConvertToText() {
  if (!bytes_loaded_)
    return "";

  StringBuilder builder;
  if (!decoder_) {
    decoder_ = TextResourceDecoder::Create(TextResourceDecoderOptions(
        TextResourceDecoderOptions::kPlainTextContent,
        encoding_.IsValid() ? encoding_ : UTF8Encoding()));
  }
  builder.Append(decoder_->Decode(static_cast<const char*>(raw_data_->Data()),
                                  raw_data_->ByteLength()));

  if (finished_loading_)
    builder.Append(decoder_->Flush());

  return builder.ToString();
}

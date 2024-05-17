String FileReaderLoader::ConvertToDataURL() {
  StringBuilder builder;
  builder.Append("data:");

  if (!bytes_loaded_)
    return builder.ToString();

  if (data_type_.IsEmpty()) {
    builder.Append("application/octet-stream");
  } else {
    builder.Append(data_type_);
  }
  builder.Append(";base64,");

  Vector<char> out;
  Base64Encode(static_cast<const char*>(raw_data_->Data()),
               raw_data_->ByteLength(), out);
  out.push_back('\0');
  builder.Append(out.data());

  return builder.ToString();
}

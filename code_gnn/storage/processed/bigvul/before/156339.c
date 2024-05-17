std::unique_ptr<TextCodec> TextCodecUTF8::Create(const TextEncoding&,
                                                 const void*) {
  return base::WrapUnique(new TextCodecUTF8());
}

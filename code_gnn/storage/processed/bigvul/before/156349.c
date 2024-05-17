void TextCodecUTF8::RegisterCodecs(TextCodecRegistrar registrar) {
  registrar("UTF-8", Create, nullptr);
}

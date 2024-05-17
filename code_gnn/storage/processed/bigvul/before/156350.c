void TextCodecUTF8::RegisterEncodingNames(EncodingNameRegistrar registrar) {
  registrar("UTF-8", "UTF-8");

  registrar("unicode11utf8", "UTF-8");
  registrar("unicode20utf8", "UTF-8");
  registrar("utf8", "UTF-8");
  registrar("x-unicode20utf8", "UTF-8");

  registrar("unicode-1-1-utf-8", "UTF-8");
}

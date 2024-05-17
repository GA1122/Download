CMYKGrayEncoder::~CMYKGrayEncoder() {
  if (str->isEncoder())
    delete str;
}

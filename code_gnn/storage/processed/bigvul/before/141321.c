Text* Document::createTextNode(const String& data) {
  return Text::Create(*this, data);
}

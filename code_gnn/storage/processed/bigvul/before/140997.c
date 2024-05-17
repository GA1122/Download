Text* Document::CreateEditingTextNode(const String& text) {
  return Text::CreateEditingText(*this, text);
}

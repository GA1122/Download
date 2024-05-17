int AXLayoutObject::textLength() const {
  if (!isTextControl())
    return -1;

  return text().length();
}

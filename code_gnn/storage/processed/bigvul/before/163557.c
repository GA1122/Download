bool XmlReader::SkipToElement() {
  do {
    switch (NodeType()) {
    case XML_READER_TYPE_ELEMENT:
      return true;
    case XML_READER_TYPE_END_ELEMENT:
      return false;
    default:
      continue;
    }
  } while (Read());
  return false;
}

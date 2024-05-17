bool XmlReader::IsClosingElement() {
  return NodeType() == XML_READER_TYPE_END_ELEMENT;
}

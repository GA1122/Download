bool XmlReader::ReadElementContent(std::string* content) {
  const int start_depth = Depth();

  if (xmlTextReaderIsEmptyElement(reader_)) {
    *content = "";
    if (!Read())
      return false;
    return true;
  }

  if (!Read())
    return false;

  while (NodeType() != XML_READER_TYPE_END_ELEMENT || Depth() != start_depth) {
    *content += XmlStringToStdString(xmlTextReaderConstValue(reader_));
    if (!Read())
      return false;
  }

  if (!Read())
    return false;

  return true;
}

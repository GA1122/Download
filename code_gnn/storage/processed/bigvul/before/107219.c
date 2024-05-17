void AutoFillXmlParser::Error(buzz::XmlParseContext* context,
                              XML_Error error_code) {
  succeeded_ = false;
}

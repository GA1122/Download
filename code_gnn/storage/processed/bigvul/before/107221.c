int AutoFillQueryXmlParser::GetIntValue(buzz::XmlParseContext* context,
                                        const char* attribute) {
  char* attr_end = NULL;
  int value = strtol(attribute, &attr_end, 10);
  if (attr_end != NULL && attr_end == attribute) {
    context->RaiseError(XML_ERROR_SYNTAX);
    return 0;
  }
  return value;
}

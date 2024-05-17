void AutoFillUploadXmlParser::StartElement(buzz::XmlParseContext* context,
                                           const char* name,
                                           const char** attrs) {
  buzz::QName qname = context->ResolveQName(name, false);
  const std::string &element = qname.LocalPart();
  if (element.compare("autofilluploadresponse") == 0) {
    while (*attrs) {
      buzz::QName attribute_qname = context->ResolveQName(attrs[0], true);
      const std::string &attribute_name = attribute_qname.LocalPart();
      if (attribute_name.compare("positiveuploadrate") == 0) {
        *positive_upload_rate_ = GetDoubleValue(context, attrs[1]);
      } else if (attribute_name.compare("negativeuploadrate") == 0) {
        *negative_upload_rate_ = GetDoubleValue(context, attrs[1]);
      }
      attrs += 2;   
    }
  }
}

void XmlConfigParser::StartElementImpl(void* ctx,
                                       const xmlChar* name,
                                       const xmlChar** atts) {
  std::string node_name(XMLCharToString(name));
  XmlConfigParser* context = FromContext(ctx);
  context->elements_.push_back(node_name);
  if (context->IsParsingData())
    context->master_prefs_.clear();
}

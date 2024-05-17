void XmlConfigParser::EndElementImpl(void* ctx, const xmlChar* name) {
  XmlConfigParser* context = FromContext(ctx);
  context->elements_.pop_back();
}

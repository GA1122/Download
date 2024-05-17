void HTMLLinkElement::LoadStylesheet(const LinkLoadParameters& params,
                                     const WTF::TextEncoding& charset,
                                     FetchParameters::DeferOption defer_option,
                                     ResourceClient* link_client) {
  return link_loader_->LoadStylesheet(params, localName(), charset,
                                      defer_option, GetDocument(), link_client);
}

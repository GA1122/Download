bool HTMLLinkElement::LoadLink(const LinkLoadParameters& params) {
  return link_loader_->LoadLink(params, GetDocument(),
                                NetworkHintsInterfaceImpl());
}

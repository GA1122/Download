void HTMLLinkElement::LinkLoaded() {
  DispatchEvent(*Event::Create(EventTypeNames::load));
}

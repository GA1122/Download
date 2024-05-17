void HTMLLinkElement::LinkLoadingErrored() {
  DispatchEvent(*Event::Create(EventTypeNames::error));
}

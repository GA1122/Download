void HTMLLinkElement::DidSendLoadForLinkPrerender() {
  DispatchEvent(*Event::Create(EventTypeNames::webkitprerenderload));
}

void HTMLLinkElement::DidStartLinkPrerender() {
  DispatchEvent(*Event::Create(EventTypeNames::webkitprerenderstart));
}

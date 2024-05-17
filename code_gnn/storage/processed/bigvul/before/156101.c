void HTMLLinkElement::DidStopLinkPrerender() {
  DispatchEvent(*Event::Create(EventTypeNames::webkitprerenderstop));
}

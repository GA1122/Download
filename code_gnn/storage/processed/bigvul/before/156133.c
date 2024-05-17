void HTMLLinkElement::Trace(blink::Visitor* visitor) {
  visitor->Trace(link_);
  visitor->Trace(sizes_);
  visitor->Trace(link_loader_);
  visitor->Trace(rel_list_);
  HTMLElement::Trace(visitor);
  LinkLoaderClient::Trace(visitor);
}

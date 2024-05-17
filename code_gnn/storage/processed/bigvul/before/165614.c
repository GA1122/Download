DOMStringList* Location::ancestorOrigins() const {
  DOMStringList* origins = DOMStringList::Create();
  if (!IsAttached())
    return origins;
  for (Frame* frame = dom_window_->GetFrame()->Tree().Parent(); frame;
       frame = frame->Tree().Parent()) {
    origins->Append(
        frame->GetSecurityContext()->GetSecurityOrigin()->ToString());
  }
  return origins;
}

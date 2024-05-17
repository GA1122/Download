void Document::BindDocumentInterfaceBroker(
    mojo::ScopedMessagePipeHandle js_handle) {
  if (!GetFrame())
    return;
  GetFrame()->BindDocumentInterfaceBroker(std::move(js_handle));
}

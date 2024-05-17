mojom::blink::DocumentInterfaceBroker* Document::GetDocumentInterfaceBroker() {
  if (!GetFrame())
    return nullptr;

  return &GetFrame()->GetDocumentInterfaceBroker();
}

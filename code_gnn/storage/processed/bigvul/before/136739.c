Document* LocalDOMWindow::CreateDocument(const String& mime_type,
                                         const DocumentInit& init,
                                         bool force_xhtml) {
  Document* document = nullptr;
  if (force_xhtml) {
    document = Document::Create(init);
  } else {
    document = DOMImplementation::createDocument(
        mime_type, init,
        init.GetFrame() ? init.GetFrame()->InViewSourceMode() : false);
    if (document->IsPluginDocument() && document->IsSandboxed(kSandboxPlugins))
      document = SinkDocument::Create(init);
  }

  return document;
}

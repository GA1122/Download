void RenderFrameImpl::DidCreateDocumentLoader(
    blink::WebDocumentLoader* document_loader) {
  DocumentState* document_state =
      DocumentState::FromDocumentLoader(document_loader);
  if (!document_state) {
    document_loader->SetExtraData(BuildDocumentState());
    document_loader->SetServiceWorkerNetworkProvider(
        ServiceWorkerNetworkProviderForFrame::CreateInvalidInstance());
  }
}

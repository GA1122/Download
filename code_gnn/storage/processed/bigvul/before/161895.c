void PrintRenderFrameHelper::DidStartProvisionalLoad(
    blink::WebDocumentLoader* document_loader) {
  is_loading_ = true;
}

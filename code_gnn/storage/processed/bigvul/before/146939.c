WebAssociatedURLLoader* WebLocalFrameImpl::CreateAssociatedURLLoader(
    const WebAssociatedURLLoaderOptions& options) {
  return new WebAssociatedURLLoaderImpl(GetFrame()->GetDocument(), options);
}

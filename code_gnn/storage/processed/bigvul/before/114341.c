bool WebGraphicsContext3DCommandBufferImpl::setParentContext(
    WebGraphicsContext3D* parent_context) {
  WebGraphicsContext3DCommandBufferImpl* parent_context_impl =
      static_cast<WebGraphicsContext3DCommandBufferImpl*>(parent_context);
  return SetParent(parent_context_impl);
}

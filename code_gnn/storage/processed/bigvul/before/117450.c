bool WebGraphicsContext3DCommandBufferImpl::setParentContext(
    WebGraphicsContext3D* parent_context) {
  WebGraphicsContext3DCommandBufferImpl* parent_context_impl =
      static_cast<WebGraphicsContext3DCommandBufferImpl*>(parent_context);
  return context_->SetParent(
      parent_context_impl ? parent_context_impl->context() : NULL);
}

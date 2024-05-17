RenderFrameImpl::GetDocumentInterfaceBroker() {
  DCHECK(document_interface_broker_.is_bound());
  return document_interface_broker_.get();
}

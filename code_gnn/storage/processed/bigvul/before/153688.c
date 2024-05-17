IdHandlerInterface* GLES2Implementation::GetIdHandler(
    SharedIdNamespaces namespace_id) const {
  return share_group_->GetIdHandler(namespace_id);
}

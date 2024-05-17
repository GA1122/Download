RangeIdHandlerInterface* GLES2Implementation::GetRangeIdHandler(
    int namespace_id) const {
  return share_group_->GetRangeIdHandler(namespace_id);
}

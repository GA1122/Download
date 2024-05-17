void RenderViewHostImpl::OnUpdateState(int32 page_id,
                                       const std::string& state) {
  delegate_->UpdateState(this, page_id, state);
}

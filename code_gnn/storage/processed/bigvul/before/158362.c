void RenderWidgetHostImpl::SetImportance(ChildProcessImportance importance) {
  if (importance_ == importance)
    return;
  importance_ = importance;
  process_->UpdateClientPriority(this);
}

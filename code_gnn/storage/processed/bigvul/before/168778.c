void RenderWidgetHostImpl::SetImportance(ChildProcessImportance importance) {
  if (importance_ == importance)
    return;
  ChildProcessImportance old = importance_;
  importance_ = importance;
  process_->UpdateWidgetImportance(old, importance_);
}

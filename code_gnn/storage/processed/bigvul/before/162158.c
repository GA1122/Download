void RenderProcessHostImpl::UpdateWidgetImportance(
    ChildProcessImportance old_value,
    ChildProcessImportance new_value) {
  DCHECK_NE(old_value, new_value);
  DCHECK(widget_importance_counts_[static_cast<size_t>(old_value)]);
  widget_importance_counts_[static_cast<size_t>(old_value)]--;
  widget_importance_counts_[static_cast<size_t>(new_value)]++;
  UpdateProcessPriority();
}

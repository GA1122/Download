ChildProcessImportance RenderProcessHostImpl::ComputeEffectiveImportance() {
  ChildProcessImportance importance = ChildProcessImportance::NORMAL;
  for (size_t i = 0u; i < arraysize(widget_importance_counts_); ++i) {
    DCHECK_GE(widget_importance_counts_[i], 0);
    if (widget_importance_counts_[i]) {
      importance = static_cast<ChildProcessImportance>(i);
    }
  }
  return importance;
}

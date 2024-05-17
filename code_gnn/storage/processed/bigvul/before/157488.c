void IsPinnedToTaskbarHelper::OnIsPinnedToTaskbarResult(
    bool succeeded,
    bool is_pinned_to_taskbar) {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);

  result_callback_.Run(succeeded, is_pinned_to_taskbar);
  delete this;
}

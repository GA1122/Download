void AutofillDialogViews::UpdatesFinished() {
  updates_scope_--;
  DCHECK_GE(updates_scope_, 0);
  if (updates_scope_ == 0 && needs_update_) {
    needs_update_ = false;
    ContentsPreferredSizeChanged();
  }
}

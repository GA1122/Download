void SelectFileDialogImpl::FileNotSelected(void* params, RunState run_state) {
  if (listener_)
    listener_->FileSelectionCanceled(params);
  EndRun(run_state);
}

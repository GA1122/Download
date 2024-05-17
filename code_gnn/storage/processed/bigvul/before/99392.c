PrintingContext::Result PrintingContext::UseDefaultSettings() {
  DCHECK(!in_print_job_);

  PRINTDLG dialog_options = { sizeof(PRINTDLG) };
  dialog_options.Flags = PD_RETURNDC | PD_RETURNDEFAULT;
  if (PrintDlg(&dialog_options) == 0) {
    ResetSettings();
    return FAILED;
  }
  return ParseDialogResult(dialog_options);
}

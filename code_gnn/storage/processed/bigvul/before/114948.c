void TestingAutomationProvider::GetShowingAppModalDialog(bool* showing_dialog,
                                                         int* dialog_button) {
  AppModalDialog* active_dialog =
      AppModalDialogQueue::GetInstance()->active_dialog();
  if (!active_dialog) {
    *showing_dialog = false;
    *dialog_button = ui::DIALOG_BUTTON_NONE;
    return;
  }
  NativeAppModalDialog* native_dialog = active_dialog->native_dialog();
  *showing_dialog = (native_dialog != NULL);
  if (*showing_dialog)
    *dialog_button = native_dialog->GetAppModalDialogButtons();
  else
    *dialog_button = ui::DIALOG_BUTTON_NONE;
}

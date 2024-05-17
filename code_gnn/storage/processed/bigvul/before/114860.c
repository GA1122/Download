void TestingAutomationProvider::ClickAppModalDialogButton(int button,
                                                          bool* success) {
  *success = false;

  NativeAppModalDialog* native_dialog =
      AppModalDialogQueue::GetInstance()->active_dialog()->native_dialog();
  if (native_dialog &&
      (native_dialog->GetAppModalDialogButtons() & button) == button) {
    if ((button & ui::DIALOG_BUTTON_OK) == ui::DIALOG_BUTTON_OK) {
      native_dialog->AcceptAppModalDialog();
      *success =  true;
    }
    if ((button & ui::DIALOG_BUTTON_CANCEL) == ui::DIALOG_BUTTON_CANCEL) {
      DCHECK(!*success) << "invalid param, OK and CANCEL specified";
      native_dialog->CancelAppModalDialog();
      *success =  true;
    }
  }
}

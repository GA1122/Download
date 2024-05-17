void AuthenticatorTouchIdIncognitoBumpSheetModel::OnAccept() {
  dialog_model()->HideDialogAndTryTouchId();
}

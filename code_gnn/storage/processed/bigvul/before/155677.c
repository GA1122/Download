void AuthenticatorSelectAccountSheetModel::OnAccept() {
  dialog_model()->OnAccountSelected(selected_);
}

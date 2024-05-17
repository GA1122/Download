void AttestationPermissionRequestSheetModel::OnAccept() {
  dialog_model()->OnAttestationPermissionResponse(true);
}

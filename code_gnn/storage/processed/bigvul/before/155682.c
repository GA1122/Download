void AttestationPermissionRequestSheetModel::OnCancel() {
  dialog_model()->OnAttestationPermissionResponse(false);
}

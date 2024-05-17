void AutofillExternalDelegate::OnPopupSuppressed() {
  manager_->DidSuppressPopup(query_form_, query_field_);
}

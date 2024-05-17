bool AutofillExternalDelegate::GetDeletionConfirmationText(
    const base::string16& value,
    int identifier,
    base::string16* title,
    base::string16* body) {
  return manager_->GetDeletionConfirmationText(value, identifier, title, body);
}

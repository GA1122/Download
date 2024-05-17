base::string16 SaveCardBubbleControllerImpl::GetExplanatoryMessage() const {
  return is_uploading_ ? l10n_util::GetStringUTF16(
                             IDS_AUTOFILL_SAVE_CARD_PROMPT_UPLOAD_EXPLANATION)
                       : base::string16();
}

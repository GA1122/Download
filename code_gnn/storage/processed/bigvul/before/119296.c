base::string16 TranslateInfoBarDelegate::GetMessageInfoBarButtonText() {
  if (step_ != translate::TRANSLATE_STEP_TRANSLATE_ERROR) {
    DCHECK_EQ(translate::TRANSLATE_STEP_TRANSLATING, step_);
  } else if ((error_type_ != TranslateErrors::IDENTICAL_LANGUAGES) &&
             (error_type_ != TranslateErrors::UNKNOWN_LANGUAGE)) {
    return l10n_util::GetStringUTF16(
        (error_type_ == TranslateErrors::UNSUPPORTED_LANGUAGE) ?
        IDS_TRANSLATE_INFOBAR_REVERT : IDS_TRANSLATE_INFOBAR_RETRY);
  }
  return base::string16();
}

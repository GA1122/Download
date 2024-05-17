const InfoBarBackground& TranslateInfoBarBase::GetBackground() const {
  return GetDelegate()->IsError() ? error_background_ : normal_background_;
}

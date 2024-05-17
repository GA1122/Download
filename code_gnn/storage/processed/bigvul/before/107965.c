InfoBar* TranslateInfoBarDelegate::CreateInfoBar() {
  TranslateInfoBarBase* infobar = NULL;
  switch (type_) {
    case BEFORE_TRANSLATE:
      infobar = new BeforeTranslateInfoBar(this);
      break;
    case AFTER_TRANSLATE:
      infobar = new AfterTranslateInfoBar(this);
      break;
    case TRANSLATING:
    case TRANSLATION_ERROR:
      infobar = new TranslateMessageInfoBar(this);
      break;
    default:
      NOTREACHED();
  }
  infobar_view_ = infobar;
  return infobar;
}

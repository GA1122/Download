BeforeTranslateInfoBar::BeforeTranslateInfoBar(
    TranslateInfoBarDelegate* delegate)
    : TranslateInfoBarBase(delegate),
      never_translate_button_(NULL),
      always_translate_button_(NULL),
      languages_menu_model_(delegate, LanguagesMenuModel::ORIGINAL),
      options_menu_model_(delegate) {
  size_t offset = 0;
  string16 text =
      l10n_util::GetStringFUTF16(IDS_TRANSLATE_INFOBAR_BEFORE_MESSAGE,
                                 string16(), &offset);

  label_1_ = CreateLabel(text.substr(0, offset));
  AddChildView(label_1_);

  label_2_ = CreateLabel(text.substr(offset));
  AddChildView(label_2_);

  accept_button_ =
    InfoBarTextButton::CreateWithMessageID(this,
                                           IDS_TRANSLATE_INFOBAR_ACCEPT);
  AddChildView(accept_button_);

  deny_button_ =
    InfoBarTextButton::CreateWithMessageID(this,
                                           IDS_TRANSLATE_INFOBAR_DENY);
  AddChildView(deny_button_);

  language_menu_button_ = CreateMenuButton(string16(), true, this);
  AddChildView(language_menu_button_);

  options_menu_button_ =
      CreateMenuButton(l10n_util::GetStringUTF16(IDS_TRANSLATE_INFOBAR_OPTIONS),
                       false, this);
  AddChildView(options_menu_button_);

  if (delegate->ShouldShowNeverTranslateButton()) {
    const string16& language = delegate->GetLanguageDisplayableNameAt(
        delegate->original_language_index());
    never_translate_button_ = InfoBarTextButton::CreateWithMessageIDAndParam(
        this, IDS_TRANSLATE_INFOBAR_NEVER_TRANSLATE, language);
    AddChildView(never_translate_button_);
  }

  if (delegate->ShouldShowAlwaysTranslateButton()) {
    const string16& language = delegate->GetLanguageDisplayableNameAt(
        delegate->original_language_index());
    always_translate_button_ = InfoBarTextButton::CreateWithMessageIDAndParam(
        this, IDS_TRANSLATE_INFOBAR_ALWAYS_TRANSLATE, language);
    AddChildView(always_translate_button_);
  }

  UpdateOriginalButtonText();
}

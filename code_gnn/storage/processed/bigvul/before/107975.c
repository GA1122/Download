 TranslateMessageInfoBar::TranslateMessageInfoBar(
     TranslateInfoBarDelegate* delegate)
    : TranslateInfoBarBase(delegate) {
  label_ = CreateLabel(delegate->GetMessageInfoBarText());
  AddChildView(label_);

  string16 button_text = delegate->GetMessageInfoBarButtonText();
  if (button_text.empty()) {
    button_ = NULL;
  } else {
    button_ = InfoBarTextButton::Create(this, button_text);
    AddChildView(button_);
  }
}

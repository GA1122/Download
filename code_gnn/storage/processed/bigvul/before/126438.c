void BrowserWindowGtk::ShowPasswordGenerationBubble(
    const gfx::Rect& rect,
    const content::PasswordForm& form,
    autofill::PasswordGenerator* password_generator) {
  WebContents* web_contents = chrome::GetActiveWebContents(browser_.get());
  if (!web_contents || !web_contents->GetContentNativeView()) {
    return;
  }

  TabContents* tab_contents = TabContents::FromWebContents(web_contents);
  if (!tab_contents)
    return;

  new PasswordGenerationBubbleGtk(rect, form, tab_contents, password_generator);
}

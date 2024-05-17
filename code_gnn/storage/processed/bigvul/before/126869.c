void BrowserView::ShowPasswordGenerationBubble(
    const gfx::Rect& rect,
    const content::PasswordForm& form,
    autofill::PasswordGenerator* password_generator) {
  gfx::Point origin(rect.origin());
  views::View::ConvertPointToScreen(GetTabContentsContainerView(), &origin);
  gfx::Rect bounds(origin, rect.size());

  WebContents* web_contents = GetActiveWebContents();
  if (!web_contents)
    return;

  PasswordGenerationBubbleView* bubble =
      new PasswordGenerationBubbleView(
          form,
          bounds,
          this,
          web_contents->GetRenderViewHost(),
          PasswordManager::FromWebContents(web_contents),
          password_generator,
          browser_.get(),
          GetWidget()->GetThemeProvider());

  views::BubbleDelegateView::CreateBubble(bubble);
  bubble->SetAlignment(views::BubbleBorder::ALIGN_ARROW_TO_MID_ANCHOR);
  bubble->Show();
}

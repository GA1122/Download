void BrowserWindowGtk::ShowAvatarBubble(WebContents* web_contents,
                                        const gfx::Rect& rect) {
  GtkWidget* widget = web_contents->GetContentNativeView();
  new AvatarMenuBubbleGtk(browser_.get(), widget,
      BubbleGtk::ARROW_LOCATION_TOP_LEFT, &rect);
}

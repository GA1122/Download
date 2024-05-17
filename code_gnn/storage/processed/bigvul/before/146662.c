views::BubbleDialogDelegateView* PageInfoBubbleView::CreatePageInfoBubble(
    Browser* browser,
    content::WebContents* web_contents,
    const GURL& url,
    const security_state::SecurityInfo& security_info) {
  views::View* anchor_view = GetPageInfoAnchorView(browser);
  gfx::Rect anchor_rect =
      anchor_view ? gfx::Rect() : GetPageInfoAnchorRect(browser);
  gfx::NativeView parent_window =
      platform_util::GetViewForWindow(browser->window()->GetNativeWindow());

  if (url.SchemeIs(content::kChromeUIScheme) ||
      url.SchemeIs(content::kChromeDevToolsScheme) ||
      url.SchemeIs(extensions::kExtensionScheme) ||
      url.SchemeIs(content::kViewSourceScheme)) {
    return new InternalPageInfoBubbleView(anchor_view, anchor_rect,
                                          parent_window, url);
  }

  return new PageInfoBubbleView(anchor_view, anchor_rect, parent_window,
                                browser->profile(), web_contents, url,
                                security_info);
}

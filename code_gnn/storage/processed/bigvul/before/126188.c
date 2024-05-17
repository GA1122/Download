gfx::Image Browser::GetCurrentPageIcon() const {
  WebContents* web_contents = chrome::GetActiveWebContents(this);
  FaviconTabHelper* favicon_tab_helper =
      web_contents ? FaviconTabHelper::FromWebContents(web_contents) : NULL;
  return favicon_tab_helper ? favicon_tab_helper->GetFavicon() : gfx::Image();
}

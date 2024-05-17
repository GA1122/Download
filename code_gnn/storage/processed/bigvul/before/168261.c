BrowserView::GetActiveTabPermissionGranter() {
  content::WebContents* web_contents = GetActiveWebContents();
  if (!web_contents)
    return nullptr;
  return extensions::TabHelper::FromWebContents(web_contents)
      ->active_tab_permission_granter();
}

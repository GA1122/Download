    BrowserWindowGtk::GetActiveTabPermissionGranter() {
  TabContents* tab = GetDisplayedTab();
  if (!tab)
    return NULL;
  return extensions::TabHelper::FromWebContents(tab->web_contents())->
      active_tab_permission_granter();
}

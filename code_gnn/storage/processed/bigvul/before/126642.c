bool TabStripModel::IsNewTabAtEndOfTabStrip(TabContents* contents) const {
  const GURL& url = contents->web_contents()->GetURL();
  return url.SchemeIs(chrome::kChromeUIScheme) &&
         url.host() == chrome::kChromeUINewTabHost &&
         contents == GetTabContentsAtImpl(count() - 1) &&
         contents->web_contents()->GetController().GetEntryCount() == 1;
}

void OneClickSigninHelper::RemoveCurrentHistoryItem(
    content::WebContents* web_contents) {
  new CurrentHistoryCleaner(web_contents);   
}

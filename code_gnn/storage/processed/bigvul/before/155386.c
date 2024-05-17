    ChromeContentBrowserClient::GetWebContentsViewDelegate(
        content::WebContents* web_contents) {
  return CreateWebContentsViewDelegate(web_contents);
}

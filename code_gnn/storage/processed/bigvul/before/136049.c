ExtensionWebContentsObserver::ExtensionWebContentsObserver(
    content::WebContents* web_contents)
    : content::WebContentsObserver(web_contents),
      browser_context_(web_contents->GetBrowserContext()),
      dispatcher_(browser_context_) {
  web_contents->ForEachFrame(
      base::Bind(&ExtensionWebContentsObserver::InitializeFrameHelper,
                 base::Unretained(this)));
  dispatcher_.set_delegate(this);
}

NavigationHandleCommitObserver::NavigationHandleCommitObserver(
    content::WebContents* web_contents,
    const GURL& url)
    : content::WebContentsObserver(web_contents),
      url_(url),
      has_committed_(false),
      was_same_document_(false),
      was_renderer_initiated_(false) {}

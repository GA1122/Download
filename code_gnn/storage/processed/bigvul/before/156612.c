  explicit WillProcessResponseObserver(content::WebContents* web_contents,
                                       const GURL& url)
      : content::WebContentsObserver(web_contents), url_(url) {}

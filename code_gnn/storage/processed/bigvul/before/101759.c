TabContents* Browser::OpenURL(const GURL& url,
                              const GURL& referrer,
                              WindowOpenDisposition disposition,
                              PageTransition::Type transition) {
  return OpenURLFromTab(NULL,
                        OpenURLParams(url, referrer, disposition, transition));
}

TabContents* Browser::OpenURLFromTab(TabContents* source,
                                     const GURL& url,
                                     const GURL& referrer,
                                     WindowOpenDisposition disposition,
                                     PageTransition::Type transition) {
  return OpenURLFromTab(source, OpenURLParams(url, referrer, disposition,
                                              transition));
}

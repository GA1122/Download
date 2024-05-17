void InProcessBrowserTest::AddTabAtIndex(
    int index,
    const GURL& url,
    content::PageTransition transition) {
  AddTabAtIndexToBrowser(browser(), index, url, transition);
}

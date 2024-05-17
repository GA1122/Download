  content::PageType NavigateAndGetPageType(const GURL& url) {
    return Navigate(url)->GetController().GetActiveEntry()->GetPageType();
  }

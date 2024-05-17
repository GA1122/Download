static bool IsNonLocalTopLevelNavigation(const GURL& url,
                                         WebFrame* frame,
                                         WebNavigationType type) {
  if (!IsTopLevelNavigation(frame))
    return false;

  if (!url.SchemeIs(chrome::kHttpScheme) && !url.SchemeIs(chrome::kHttpsScheme))
    return false;

  if (type != WebKit::WebNavigationTypeReload &&
      type != WebKit::WebNavigationTypeFormSubmitted &&
      type != WebKit::WebNavigationTypeFormResubmitted &&
      type != WebKit::WebNavigationTypeBackForward) {
    WebKit::WebFrame* opener = frame->opener();
    if (!opener) {
      return true;
    }

    if (url.GetOrigin() != GURL(opener->document().url()).GetOrigin())
      return true;
  }
  return false;
}

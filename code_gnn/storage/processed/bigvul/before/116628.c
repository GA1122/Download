bool RenderViewImpl::IsNonLocalTopLevelNavigation(
    const GURL& url, WebKit::WebFrame* frame, WebKit::WebNavigationType type) {
  if (frame->parent() != NULL)
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
    } else {
      if (url.GetOrigin() != GURL(opener->document().url()).GetOrigin())
        return true;
    }
  }
  return false;
}

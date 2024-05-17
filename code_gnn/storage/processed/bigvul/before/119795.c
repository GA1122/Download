void NavigationControllerImpl::LoadURL(
    const GURL& url,
    const Referrer& referrer,
    PageTransition transition,
    const std::string& extra_headers) {
  LoadURLParams params(url);
  params.referrer = referrer;
  params.transition_type = transition;
  params.extra_headers = extra_headers;
  LoadURLWithParams(params);
}

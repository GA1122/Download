bool IsBrowserInitiated(NavigationParams* pending) {
  return pending &&
         !pending->common_params.url.SchemeIs(url::kJavaScriptScheme);
}

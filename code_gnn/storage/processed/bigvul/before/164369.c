bool TabsUpdateFunction::UpdateURL(const std::string& url_string,
                                   int tab_id,
                                   std::string* error) {
  GURL url =
      ExtensionTabUtil::ResolvePossiblyRelativeURL(url_string, extension());

  if (!url.is_valid()) {
    *error = ErrorUtils::FormatErrorMessage(tabs_constants::kInvalidUrlError,
                                            url_string);
    return false;
  }

  if (ExtensionTabUtil::IsKillURL(url)) {
    *error = tabs_constants::kNoCrashBrowserError;
    return false;
  }

  const bool is_javascript_scheme = url.SchemeIs(url::kJavaScriptScheme);
  UMA_HISTOGRAM_BOOLEAN("Extensions.ApiTabUpdateJavascript",
                        is_javascript_scheme);
  if (is_javascript_scheme) {
    *error = tabs_constants::kJavaScriptUrlsNotAllowedInTabsUpdate;
    return false;
  }

  bool use_renderer_initiated = false;
  if (extension() && extension()->id() == extension_misc::kPdfExtensionId)
    use_renderer_initiated = true;
  NavigationController::LoadURLParams load_params(url);
  load_params.is_renderer_initiated = use_renderer_initiated;
  web_contents_->GetController().LoadURLWithParams(load_params);

  DCHECK_EQ(url,
            web_contents_->GetController().GetPendingEntry()->GetVirtualURL());

  return true;
}

WebContents* DevToolsWindow::OpenURLFromTab(
    WebContents* source,
    const content::OpenURLParams& params) {
  DCHECK(source == main_web_contents_);
  if (!params.url.SchemeIs(content::kChromeDevToolsScheme)) {
    WebContents* inspected_web_contents = GetInspectedWebContents();
    return inspected_web_contents ?
        inspected_web_contents->OpenURL(params) : NULL;
  }

  bindings_->Reattach();

  content::NavigationController::LoadURLParams load_url_params(params.url);
  main_web_contents_->GetController().LoadURLWithParams(load_url_params);
  return main_web_contents_;
}

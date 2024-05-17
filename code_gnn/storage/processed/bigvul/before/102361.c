bool ExtensionHelper::InstallWebApplicationUsingDefinitionFile(
    WebFrame* frame, string16* error) {
  if (!CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kEnableCrxlessWebApps)) {
    *error = ASCIIToUTF16("CRX-less web apps aren't enabled.");
    return false;
  }

  if (frame != frame->top()) {
    *error = ASCIIToUTF16("Applications can only be installed from the top "
                          "frame.");
    return false;
  }

  if (pending_app_info_.get()) {
    *error = ASCIIToUTF16("An application install is already in progress.");
    return false;
  }

  pending_app_info_.reset(new WebApplicationInfo());
  if (!web_apps::ParseWebAppFromWebDocument(frame, pending_app_info_.get(),
                                            error)) {
    return false;
  }

  if (!pending_app_info_->manifest_url.is_valid()) {
    *error = ASCIIToUTF16("Web application definition not found or invalid.");
    return false;
  }

  app_definition_fetcher_.reset(new ResourceFetcher(
      pending_app_info_->manifest_url, render_view()->GetWebView()->mainFrame(),
      WebURLRequest::TargetIsSubresource,
      base::Bind(&ExtensionHelper::DidDownloadApplicationDefinition,
                 base::Unretained(this))));
  return true;
 }

Profile* GetSourceProfile(browser::NavigateParams* params,
    Browser* source_browser) {
  if (params->source_contents)
    return params->source_contents->profile();

  if (source_browser)
    return source_browser->profile();

  if (params->profile)
    return params->profile;

  return params->browser->profile();
}

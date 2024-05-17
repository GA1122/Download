void BrowserWindowGtk::ConfirmAddSearchProvider(TemplateURL* template_url,
                                                Profile* profile) {
  new EditSearchEngineDialog(window_, template_url, NULL, profile);
}

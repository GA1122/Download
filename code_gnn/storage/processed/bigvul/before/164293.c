WebContents* PageCaptureSaveAsMHTMLFunction::GetWebContents() {
  Browser* browser = NULL;
  content::WebContents* web_contents = NULL;

  if (!ExtensionTabUtil::GetTabById(params_->details.tab_id, GetProfile(),
                                    include_incognito_information(), &browser,
                                    NULL, &web_contents, NULL)) {
    return NULL;
  }
  return web_contents;
}

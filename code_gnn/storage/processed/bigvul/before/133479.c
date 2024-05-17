void AddMediaRouterUIResources(content::WebUIDataSource* html_source) {
  AddIcons(html_source);
  AddMainWebResources(html_source);
  AddPolymerElements(html_source);
  html_source->SetDefaultResource(IDR_MEDIA_ROUTER_HTML);
}

void ExtensionHelper::OnGetApplicationInfo(int page_id) {
  WebApplicationInfo app_info;
  if (page_id == render_view()->GetPageId()) {
    string16 error;
    web_apps::ParseWebAppFromWebDocument(
        render_view()->GetWebView()->mainFrame(), &app_info, &error);
  }

  for (size_t i = 0; i < app_info.icons.size(); ++i) {
    if (app_info.icons[i].url.SchemeIs(chrome::kDataScheme)) {
      app_info.icons.erase(app_info.icons.begin() + i);
      --i;
    }
  }

  Send(new ExtensionHostMsg_DidGetApplicationInfo(
      routing_id(), page_id, app_info));
}

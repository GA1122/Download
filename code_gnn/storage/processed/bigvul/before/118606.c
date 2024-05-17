void AppLauncherHandler::SetAppToBeHighlighted() {
  if (highlight_app_id_.empty())
    return;

  base::StringValue app_id(highlight_app_id_);
  web_ui()->CallJavascriptFunction("ntp.setAppToBeHighlighted", app_id);
  highlight_app_id_.clear();
}

bool TabsGetZoomSettingsFunction::RunAsync() {
  std::unique_ptr<tabs::GetZoomSettings::Params> params(
      tabs::GetZoomSettings::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params);

  int tab_id = params->tab_id ? *params->tab_id : -1;
  WebContents* web_contents = GetWebContents(tab_id);
  if (!web_contents)
    return false;
  ZoomController* zoom_controller =
      ZoomController::FromWebContents(web_contents);

  ZoomController::ZoomMode zoom_mode = zoom_controller->zoom_mode();
  api::tabs::ZoomSettings zoom_settings;
  ZoomModeToZoomSettings(zoom_mode, &zoom_settings);
  zoom_settings.default_zoom_factor.reset(new double(
      content::ZoomLevelToZoomFactor(zoom_controller->GetDefaultZoomLevel())));

  results_ = api::tabs::GetZoomSettings::Results::Create(zoom_settings);
  SendResponse(true);
  return true;
}

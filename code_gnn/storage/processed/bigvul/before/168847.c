bool TabsSetZoomFunction::RunAsync() {
  std::unique_ptr<tabs::SetZoom::Params> params(
      tabs::SetZoom::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params);

  int tab_id = params->tab_id ? *params->tab_id : -1;
  WebContents* web_contents = GetWebContents(tab_id);
  if (!web_contents)
    return false;

  GURL url(web_contents->GetVisibleURL());
  if (PermissionsData::IsRestrictedUrl(url, extension(), &error_))
    return false;

  ZoomController* zoom_controller =
      ZoomController::FromWebContents(web_contents);
  double zoom_level = params->zoom_factor > 0
                          ? content::ZoomFactorToZoomLevel(params->zoom_factor)
                          : zoom_controller->GetDefaultZoomLevel();

  scoped_refptr<ExtensionZoomRequestClient> client(
      new ExtensionZoomRequestClient(extension()));
  if (!zoom_controller->SetZoomLevelByClient(zoom_level, client)) {
    error_ = keys::kCannotZoomDisabledTabError;
    return false;
  }

  SendResponse(true);
  return true;
}

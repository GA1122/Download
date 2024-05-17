Response EmulationHandler::ClearGeolocationOverride() {
  if (!GetWebContents())
    return Response::InternalError();

  auto* geolocation_context = GetWebContents()->GetGeolocationContext();
  geolocation_context->ClearOverride();
  return Response::OK();
}

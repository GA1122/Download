Response EmulationHandler::SetVisibleSize(int width, int height) {
  if (width < 0 || height < 0)
    return Response::InvalidParams("Width and height must be non-negative");

  if (GetWebContents())
    GetWebContents()->SetDeviceEmulationSize(gfx::Size(width, height));
  else
    return Response::Error("Can't find the associated web contents");

  return Response::OK();
}

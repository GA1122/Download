Response PageHandler::StopScreencast() {
  screencast_enabled_ = false;
  return Response::FallThrough();
}

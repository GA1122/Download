Response NetworkHandler::SetBypassServiceWorker(bool bypass) {
  bypass_service_worker_ = bypass;
  return Response::FallThrough();
}

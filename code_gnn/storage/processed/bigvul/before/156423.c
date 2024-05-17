Response PageHandler::StopScreencast() {
  screencast_enabled_ = false;
  if (video_consumer_)
    video_consumer_->StopCapture();
  return Response::FallThrough();
}

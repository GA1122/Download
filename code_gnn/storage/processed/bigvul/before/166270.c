void LogVideoCaptureEvent(VideoCaptureEvent event) {
  UMA_HISTOGRAM_ENUMERATION("Media.VideoCaptureManager.Event", event,
                            NUM_VIDEO_CAPTURE_EVENT);
}

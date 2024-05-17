void LogVideoCaptureError(media::VideoCaptureError error) {
  UMA_HISTOGRAM_ENUMERATION(
      "Media.VideoCapture.Error", error,
      static_cast<int>(media::VideoCaptureError::kMaxValue) + 1);
}

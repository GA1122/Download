void TabsCaptureVisibleTabFunction::OnCaptureSuccess(const SkBitmap& bitmap) {
  std::string base64_result;
  if (!EncodeBitmap(bitmap, &base64_result)) {
    OnCaptureFailure(FAILURE_REASON_ENCODING_FAILED);
    return;
  }

  Respond(OneArgument(std::make_unique<base::Value>(base64_result)));
}

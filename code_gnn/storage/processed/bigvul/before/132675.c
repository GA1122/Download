void BlinkTestRunner::SetDeviceScaleFactor(float factor) {
  content::SetDeviceScaleFactor(render_view(), factor);
}

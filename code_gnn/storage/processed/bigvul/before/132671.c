void BlinkTestRunner::SetDeviceColorProfile(const std::string& name) {
  content::SetDeviceColorProfile(render_view(), name);
}

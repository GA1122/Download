void LoginDisplayHostWebUI::OnTouchscreenDeviceConfigurationChanged() {
  if (GetOobeUI())
    GetOobeUI()->OnDisplayConfigurationChanged();
}

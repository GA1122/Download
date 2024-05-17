void LoginDisplayHostWebUI::OnDisplayAdded(
    const display::Display& new_display) {
  if (GetOobeUI())
    GetOobeUI()->OnDisplayConfigurationChanged();
}

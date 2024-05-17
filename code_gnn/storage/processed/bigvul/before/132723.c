void ChromotingInstance::HandleNotifyClientResolution(
    const base::DictionaryValue& data) {
  int width = 0;
  int height = 0;
  int x_dpi = kDefaultDPI;
  int y_dpi = kDefaultDPI;
  if (!data.GetInteger("width", &width) ||
      !data.GetInteger("height", &height) ||
      !data.GetInteger("x_dpi", &x_dpi) ||
      !data.GetInteger("y_dpi", &y_dpi) ||
      width <= 0 || height <= 0 ||
      x_dpi <= 0 || y_dpi <= 0) {
    LOG(ERROR) << "Invalid notifyClientResolution.";
    return;
  }

  if (!IsConnected()) {
    return;
  }

  protocol::ClientResolution client_resolution;
  client_resolution.set_width(width);
  client_resolution.set_height(height);
  client_resolution.set_x_dpi(x_dpi);
  client_resolution.set_y_dpi(y_dpi);

  client_resolution.set_dips_width((width * kDefaultDPI) / x_dpi);
  client_resolution.set_dips_height((height * kDefaultDPI) / y_dpi);

  client_->host_stub()->NotifyClientResolution(client_resolution);
}

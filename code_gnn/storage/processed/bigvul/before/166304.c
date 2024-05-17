  void OnDeviceOpened(base::Closure quit_closure,
                      bool success,
                      const std::string& label,
                      const MediaStreamDevice& opened_device) {
    if (success) {
      opened_device_label_ = label;
      opened_session_id_ = opened_device.session_id;
    }
    std::move(quit_closure).Run();
  }

  void OnOpenDevice(int page_request_id,
                    const std::string& device_id,
                    MediaStreamType type,
                    const base::Closure& quit_closure) {
    quit_closures_.push(quit_closure);
    MediaStreamDispatcherHost::OpenDevice(
        page_request_id, device_id, type,
        base::BindOnce(&MockMediaStreamDispatcherHost::OnDeviceOpened,
                       base::Unretained(this)));
  }

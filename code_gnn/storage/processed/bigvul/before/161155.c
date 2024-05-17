  void RunMojoCallbacks() {
    if (generate_stream_cb) {
      std::move(generate_stream_cb)
          .Run(MEDIA_DEVICE_FAILED_DUE_TO_SHUTDOWN, std::string(),
               MediaStreamDevices(), MediaStreamDevices());
    }

    if (open_device_cb) {
      std::move(open_device_cb)
          .Run(false  , std::string(), MediaStreamDevice());
    }
  }

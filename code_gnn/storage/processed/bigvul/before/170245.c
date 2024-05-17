  MediaStreamDevicesControllerBrowserTest()
      : request_url_allowed_via_whitelist_(false) {
    policy_value_ = GetParam();
  }

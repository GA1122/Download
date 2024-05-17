  void CreateWithoutWaitingForAuth(const std::string& device_id) {
    Create(device_id, url::Origin(GURL(kSecurityOrigin)), false, false);
  }

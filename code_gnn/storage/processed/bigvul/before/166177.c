  MediaDeviceSaltAndOrigin GetSaltAndOrigin(int  ,
                                            int  ) {
    return MediaDeviceSaltAndOrigin(browser_context_->GetMediaDeviceIDSalt(),
                                    "fake_group_id_salt", origin_);
  }

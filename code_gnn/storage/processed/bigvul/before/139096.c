  void Create(const std::string& device_id,
              const url::Origin& security_origin,
              bool wait_for_auth,
              bool expect_onauthorized) {
    media::OutputDeviceStatus expected_device_status =
        device_id == kDefaultDeviceId ||
                device_id ==
                    MediaStreamManager::GetHMACForMediaDeviceID(
                        kSalt, url::Origin(GURL(kSecurityOrigin)),
                        GetNondefaultIdExpectedToPassPermissionsCheck())
            ? media::OUTPUT_DEVICE_STATUS_OK
            : device_id == kBadDeviceId
                  ? media::OUTPUT_DEVICE_STATUS_ERROR_NOT_AUTHORIZED
                  : media::OUTPUT_DEVICE_STATUS_ERROR_NOT_FOUND;

    if (expect_onauthorized)
      EXPECT_CALL(*host_.get(),
                  OnDeviceAuthorized(kStreamId, expected_device_status, _, _));

    if (expected_device_status == media::OUTPUT_DEVICE_STATUS_OK) {
      EXPECT_CALL(*host_.get(), WasNotifiedOfCreation(kStreamId, _));
      EXPECT_CALL(mirroring_manager_, AddDiverter(render_process_host_.GetID(),
                                                  kRenderFrameId, NotNull()))
          .RetiresOnSaturation();
    }

    media::AudioParameters params(
        media::AudioParameters::AUDIO_FAKE, media::CHANNEL_LAYOUT_STEREO,
        media::AudioParameters::kAudioCDSampleRate, 16,
        media::AudioParameters::kAudioCDSampleRate / 10);
    int session_id = 0;

    host_->OnRequestDeviceAuthorization(kStreamId, kRenderFrameId, session_id,
                                        device_id, security_origin);
    if (wait_for_auth)
      auth_run_loop_.Run();

    if (!wait_for_auth ||
        expected_device_status == media::OUTPUT_DEVICE_STATUS_OK)
      host_->OnCreateStream(kStreamId, kRenderFrameId, params);

    if (expected_device_status == media::OUTPUT_DEVICE_STATUS_OK)
      EXPECT_CALL(mirroring_manager_, RemoveDiverter(NotNull()))
          .RetiresOnSaturation();
    SyncWithAudioThread();
  }

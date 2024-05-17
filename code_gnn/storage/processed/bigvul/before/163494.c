void PushMessagingServiceImpl::DoSubscribe(
    const PushMessagingAppIdentifier& app_identifier,
    const content::PushSubscriptionOptions& options,
    const RegisterCallback& register_callback,
    ContentSetting content_setting) {
  if (content_setting != CONTENT_SETTING_ALLOW) {
    SubscribeEndWithError(
        register_callback,
        content::mojom::PushRegistrationStatus::PERMISSION_DENIED);
    return;
  }

  IncreasePushSubscriptionCount(1, true  );

  GetInstanceIDDriver()
      ->GetInstanceID(app_identifier.app_id())
      ->GetToken(NormalizeSenderInfo(options.sender_info), kGCMScope,
                 std::map<std::string, std::string>()  ,
                 base::Bind(&PushMessagingServiceImpl::DidSubscribe,
                            weak_factory_.GetWeakPtr(), app_identifier,
                            options.sender_info, register_callback));
}

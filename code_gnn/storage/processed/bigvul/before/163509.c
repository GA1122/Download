void PushMessagingServiceImpl::OnMessage(const std::string& app_id,
                                         const gcm::IncomingMessage& message) {
  if (g_browser_process->IsShuttingDown() || shutdown_started_)
    return;

  in_flight_message_deliveries_.insert(app_id);

#if BUILDFLAG(ENABLE_BACKGROUND)
  if (g_browser_process->background_mode_manager()) {
    UMA_HISTOGRAM_BOOLEAN("PushMessaging.ReceivedMessageInBackground",
                          g_browser_process->background_mode_manager()
                              ->IsBackgroundWithoutWindows());
  }

  if (!in_flight_keep_alive_) {
    in_flight_keep_alive_ = std::make_unique<ScopedKeepAlive>(
        KeepAliveOrigin::IN_FLIGHT_PUSH_MESSAGE,
        KeepAliveRestartOption::DISABLED);
  }
#endif

  base::Closure message_handled_closure =
      message_callback_for_testing_.is_null() ? base::Bind(&base::DoNothing)
                                              : message_callback_for_testing_;
  PushMessagingAppIdentifier app_identifier =
      PushMessagingAppIdentifier::FindByAppId(profile_, app_id);
  if (app_identifier.is_null()) {
    DeliverMessageCallback(app_id, GURL::EmptyGURL(),
                           -1  ,
                           message, message_handled_closure,
                           content::mojom::PushDeliveryStatus::UNKNOWN_APP_ID);
    return;
  }
  if (!IsPermissionSet(app_identifier.origin())) {
    DeliverMessageCallback(
        app_id, app_identifier.origin(),
        app_identifier.service_worker_registration_id(), message,
        message_handled_closure,
        content::mojom::PushDeliveryStatus::PERMISSION_DENIED);
    return;
  }

  rappor::SampleDomainAndRegistryFromGURL(
      g_browser_process->rappor_service(),
      "PushMessaging.MessageReceived.Origin", app_identifier.origin());

  content::PushEventPayload payload;
  if (message.decrypted)
    payload.setData(message.raw_data);

  content::BrowserContext::DeliverPushMessage(
      profile_, app_identifier.origin(),
      app_identifier.service_worker_registration_id(), payload,
      base::Bind(&PushMessagingServiceImpl::DeliverMessageCallback,
                 weak_factory_.GetWeakPtr(), app_identifier.app_id(),
                 app_identifier.origin(),
                 app_identifier.service_worker_registration_id(), message,
                 message_handled_closure));

  if (!message_dispatched_callback_for_testing_.is_null()) {
    message_dispatched_callback_for_testing_.Run(
        app_id, app_identifier.origin(),
        app_identifier.service_worker_registration_id(), payload);
  }
}

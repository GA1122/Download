void PushMessagingServiceImpl::Observe(
    int type,
    const content::NotificationSource& source,
    const content::NotificationDetails& details) {
  DCHECK_EQ(chrome::NOTIFICATION_APP_TERMINATING, type);
  shutdown_started_ = true;
#if BUILDFLAG(ENABLE_BACKGROUND)
  in_flight_keep_alive_.reset();
#endif   
}

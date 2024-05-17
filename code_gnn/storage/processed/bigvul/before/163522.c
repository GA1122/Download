void PushMessagingServiceImpl::Shutdown() {
  GetGCMDriver()->RemoveAppHandler(kPushMessagingAppIdentifierPrefix);
  HostContentSettingsMapFactory::GetForProfile(profile_)->RemoveObserver(this);
}

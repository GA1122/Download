PushMessagingServiceObserver::Create() {
#if defined(OS_ANDROID)
  return std::make_unique<PushMessagingServiceObserverAndroid>();
#else
  return nullptr;
#endif
}

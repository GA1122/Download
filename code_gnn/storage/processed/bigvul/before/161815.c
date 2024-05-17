PlatformSensorProviderMac* PlatformSensorProviderMac::GetInstance() {
  return base::Singleton<
      PlatformSensorProviderMac,
      base::LeakySingletonTraits<PlatformSensorProviderMac>>::get();
}

PlatformSensorProviderAndroid* PlatformSensorProviderAndroid::GetInstance() {
  return base::Singleton<
      PlatformSensorProviderAndroid,
      base::LeakySingletonTraits<PlatformSensorProviderAndroid>>::get();
}

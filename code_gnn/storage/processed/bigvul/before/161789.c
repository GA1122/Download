PlatformSensorProviderAndroid::PlatformSensorProviderAndroid() {
  JNIEnv* env = AttachCurrentThread();
  j_object_.Reset(Java_PlatformSensorProvider_create(env));
}

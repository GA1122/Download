void PlatformSensorProviderAndroid::SetSensorManagerToNullForTesting() {
  JNIEnv* env = AttachCurrentThread();
  Java_PlatformSensorProvider_setSensorManagerToNullForTesting(env, j_object_);
}

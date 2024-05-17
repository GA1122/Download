PlatformSensorConfiguration PlatformSensorAndroid::GetDefaultConfiguration() {
  JNIEnv* env = AttachCurrentThread();
  jdouble frequency =
      Java_PlatformSensor_getDefaultConfiguration(env, j_object_);
  return PlatformSensorConfiguration(frequency);
}

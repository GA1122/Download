bool PlatformSensorAndroid::StartSensor(
    const PlatformSensorConfiguration& configuration) {
  JNIEnv* env = AttachCurrentThread();
  return Java_PlatformSensor_startSensor(env, j_object_,
                                         configuration.frequency());
}

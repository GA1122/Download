PlatformSensorAndroid::~PlatformSensorAndroid() {
  JNIEnv* env = AttachCurrentThread();
  Java_PlatformSensor_sensorDestroyed(env, j_object_);
}

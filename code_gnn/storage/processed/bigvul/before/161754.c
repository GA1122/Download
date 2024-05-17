mojom::ReportingMode PlatformSensorAndroid::GetReportingMode() {
  JNIEnv* env = AttachCurrentThread();
  return static_cast<mojom::ReportingMode>(
      Java_PlatformSensor_getReportingMode(env, j_object_));
}

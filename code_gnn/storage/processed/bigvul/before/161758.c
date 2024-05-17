void PlatformSensorAndroid::UpdatePlatformSensorReading(
    JNIEnv*,
    const base::android::JavaRef<jobject>& caller,
    jdouble timestamp,
    jdouble value1,
    jdouble value2,
    jdouble value3,
    jdouble value4) {
  SensorReading reading;
  reading.raw.timestamp = timestamp;
  reading.raw.values[0] = value1;
  reading.raw.values[1] = value2;
  reading.raw.values[2] = value3;
  reading.raw.values[3] = value4;

  UpdateSharedBufferAndNotifyClients(reading);
}

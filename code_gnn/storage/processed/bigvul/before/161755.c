void PlatformSensorAndroid::NotifyPlatformSensorError(
    JNIEnv*,
    const JavaRef<jobject>& caller) {
  task_runner_->PostTask(
      FROM_HERE, base::Bind(&PlatformSensorAndroid::NotifySensorError, this));
}

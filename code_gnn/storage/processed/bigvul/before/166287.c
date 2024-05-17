void VideoCaptureManager::RegisterListener(
    MediaStreamProviderListener* listener) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  DCHECK(listener);
  listeners_.AddObserver(listener);
#if defined(OS_ANDROID)
  application_state_has_running_activities_ = true;
  app_status_listener_ = base::android::ApplicationStatusListener::New(
      base::BindRepeating(&VideoCaptureManager::OnApplicationStateChange,
                          base::Unretained(this)));
#endif
}

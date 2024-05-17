static void OnRequestFileAccessResult(JNIEnv* env,
                                      const JavaParamRef<jobject>& obj,
                                      jlong callback_id,
                                      jboolean granted) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  DCHECK(callback_id);

  std::unique_ptr<
      DownloadControllerBase::AcquireFileAccessPermissionCallback>
  cb(reinterpret_cast<
      DownloadControllerBase::AcquireFileAccessPermissionCallback*>(
      callback_id));
  if (!granted) {
    DownloadController::RecordDownloadCancelReason(
        DownloadController::CANCEL_REASON_NO_STORAGE_PERMISSION);
  }
  cb->Run(granted);
}

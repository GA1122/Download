void ConnectionInfoPopupAndroid::ResetCertDecisions(
    JNIEnv* env,
    jobject obj,
    jobject java_web_contents) {
  presenter_->OnRevokeSSLErrorBypassButtonPressed();
}

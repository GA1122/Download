void PasswordGenerationDialogViewAndroid::PasswordRejected(
    JNIEnv* env,
    const base::android::JavaParamRef<jobject>& obj) {
  controller_->GeneratedPasswordRejected();
}

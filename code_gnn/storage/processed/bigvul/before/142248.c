void PasswordGenerationDialogViewAndroid::PasswordAccepted(
    JNIEnv* env,
    const base::android::JavaParamRef<jobject>& obj,
    const base::android::JavaParamRef<jstring>& password) {
  controller_->GeneratedPasswordAccepted(
      base::android::ConvertJavaStringToUTF16(env, password));
}

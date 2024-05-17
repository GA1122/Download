 void ChromeHttpAuthHandler::OnAutofillDataAvailable(
     const base::string16& username,
     const base::string16& password) {
  DCHECK(java_chrome_http_auth_handler_.obj() != NULL);
  JNIEnv* env = base::android::AttachCurrentThread();
  ScopedJavaLocalRef<jstring> j_username =
      ConvertUTF16ToJavaString(env, username);
  ScopedJavaLocalRef<jstring> j_password =
      ConvertUTF16ToJavaString(env, password);
  Java_ChromeHttpAuthHandler_onAutofillDataAvailable(
      env, java_chrome_http_auth_handler_, j_username, j_password);
}

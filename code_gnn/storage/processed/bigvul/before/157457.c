ScopedJavaLocalRef<jstring> ChromeHttpAuthHandler::GetMessageBody(
    JNIEnv* env,
    const JavaParamRef<jobject>&) {
  if (explanation_.empty())
    return ConvertUTF16ToJavaString(env, authority_);
  return ConvertUTF16ToJavaString(
      env, authority_ + base::ASCIIToUTF16(" ") + explanation_);
}

static ScopedJavaLocalRef<jstring> GetDownloadWarningText(
    JNIEnv* env,
    const JavaParamRef<jclass>& clazz,
    const JavaParamRef<jstring>& filename) {
  return base::android::ConvertUTF8ToJavaString(
      env, l10n_util::GetStringFUTF8(
               IDS_PROMPT_DANGEROUS_DOWNLOAD,
               base::android::ConvertJavaStringToUTF16(env, filename)));
}

void SetLocale(JNIEnv* env,
               const JavaParamRef<jclass>&,
               const JavaParamRef<jstring>& locale) {
  g_locale = ConvertJavaStringToUTF8(env, locale);
}

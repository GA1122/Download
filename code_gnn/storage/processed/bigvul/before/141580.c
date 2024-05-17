void JNI_ForeignSessionHelper_CopyWindowToJava(
    JNIEnv* env,
    const sessions::SessionWindow& window,
    ScopedJavaLocalRef<jobject>& j_window) {
  for (const auto& tab_ptr : window.tabs) {
    const sessions::SessionTab& session_tab = *(tab_ptr.get());

    if (ShouldSkipTab(session_tab))
      return;

    JNI_ForeignSessionHelper_CopyTabToJava(env, session_tab, j_window);
  }
}

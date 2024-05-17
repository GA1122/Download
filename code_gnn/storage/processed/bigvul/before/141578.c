void JNI_ForeignSessionHelper_CopySessionToJava(
    JNIEnv* env,
    const SyncedSession& session,
    ScopedJavaLocalRef<jobject>& j_session) {
  for (const auto& window_pair : session.windows) {
    const sessions::SessionWindow& window = window_pair.second->wrapped_window;

    if (ShouldSkipWindow(window))
      continue;

    ScopedJavaLocalRef<jobject> last_pushed_window;
    last_pushed_window.Reset(Java_ForeignSessionHelper_pushWindow(
        env, j_session, window.timestamp.ToJavaTime(), window.window_id.id()));

    JNI_ForeignSessionHelper_CopyWindowToJava(env, window, last_pushed_window);
  }
}

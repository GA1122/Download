void JNI_ForeignSessionHelper_CopyTabToJava(
    JNIEnv* env,
    const sessions::SessionTab& tab,
    ScopedJavaLocalRef<jobject>& j_window) {
  int selected_index = tab.normalized_navigation_index();
  DCHECK_GE(selected_index, 0);
  DCHECK_LT(selected_index, static_cast<int>(tab.navigations.size()));

  const sessions::SerializedNavigationEntry& current_navigation =
      tab.navigations.at(selected_index);

  GURL tab_url = current_navigation.virtual_url();

  Java_ForeignSessionHelper_pushTab(
      env, j_window, ConvertUTF8ToJavaString(env, tab_url.spec()),
      ConvertUTF16ToJavaString(env, current_navigation.title()),
      tab.timestamp.ToJavaTime(), tab.tab_id.id());
}

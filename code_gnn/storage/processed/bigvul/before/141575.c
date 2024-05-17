jboolean ForeignSessionHelper::GetForeignSessions(
    JNIEnv* env,
    const JavaParamRef<jobject>& obj,
    const JavaParamRef<jobject>& result) {
  OpenTabsUIDelegate* open_tabs = GetOpenTabsUIDelegate(profile_);
  if (!open_tabs)
    return false;

  std::vector<const SyncedSession*> sessions;
  if (!open_tabs->GetAllForeignSessions(&sessions))
    return false;

  DictionaryPrefUpdate pref_update(profile_->GetPrefs(),
                                   prefs::kNtpCollapsedForeignSessions);
  base::DictionaryValue* pref_collapsed_sessions = pref_update.Get();
  std::unique_ptr<base::DictionaryValue> collapsed_sessions(
      pref_collapsed_sessions->DeepCopy());
  pref_collapsed_sessions->Clear();

  ScopedJavaLocalRef<jobject> last_pushed_session;

  for (size_t i = 0; i < sessions.size(); ++i) {
    const SyncedSession& session = *(sessions[i]);
    if (ShouldSkipSession(session))
      continue;

    const bool is_collapsed = collapsed_sessions->HasKey(session.session_tag);

    if (is_collapsed)
      pref_collapsed_sessions->SetBoolean(session.session_tag, true);

    last_pushed_session.Reset(Java_ForeignSessionHelper_pushSession(
        env, result, ConvertUTF8ToJavaString(env, session.session_tag),
        ConvertUTF8ToJavaString(env, session.session_name), session.device_type,
        session.modified_time.ToJavaTime()));

    JNI_ForeignSessionHelper_CopySessionToJava(env, session,
                                               last_pushed_session);
  }

  return true;
}

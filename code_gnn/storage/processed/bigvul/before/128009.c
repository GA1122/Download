void AwContents::GrantFileSchemeAccesstoChildProcess(JNIEnv* env, jobject obj) {
  content::ChildProcessSecurityPolicy::GetInstance()->GrantScheme(
      web_contents_->GetRenderProcessHost()->GetID(), url::kFileScheme);
}

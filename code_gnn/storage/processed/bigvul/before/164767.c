void ChildProcessLauncherHelper::SetProcessPriorityOnLauncherThread(
    base::Process process,
    const ChildProcessLauncherPriority& priority) {
  JNIEnv* env = AttachCurrentThread();
  DCHECK(env);
  return Java_ChildProcessLauncherHelperImpl_setPriority(
      env, java_peer_, process.Handle(), priority.visible,
      priority.has_media_stream, priority.has_foreground_service_worker,
      priority.frame_depth, priority.intersects_viewport,
      priority.boost_for_pending_views, static_cast<jint>(priority.importance));
}

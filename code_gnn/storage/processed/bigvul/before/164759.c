void ChildProcessLauncherHelper::DumpProcessStack(
    const base::Process& process) {
  JNIEnv* env = AttachCurrentThread();
  DCHECK(env);
  return Java_ChildProcessLauncherHelperImpl_dumpProcessStack(env, java_peer_,
                                                              process.Handle());
}

void ChildProcessLauncherHelper::OnChildProcessStarted(
    JNIEnv*,
    const base::android::JavaParamRef<jobject>& obj,
    jint handle) {
  DCHECK(CurrentlyOnProcessLauncherTaskRunner());
  scoped_refptr<ChildProcessLauncherHelper> ref(this);
  Release();   

  int launch_result = (handle == base::kNullProcessHandle)
                          ? LAUNCH_RESULT_FAILURE
                          : LAUNCH_RESULT_SUCCESS;

  ChildProcessLauncherHelper::Process process;
  process.process = base::Process(handle);
  PostLaunchOnLauncherThread(std::move(process), launch_result);
}

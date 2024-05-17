ChildProcessLauncherHelper::LaunchProcessOnLauncherThread(
    const base::LaunchOptions& options,
    std::unique_ptr<PosixFileDescriptorInfo> files_to_register,
    bool* is_synchronous_launch,
    int* launch_result) {
  *is_synchronous_launch = false;

  JNIEnv* env = AttachCurrentThread();
  DCHECK(env);

  ScopedJavaLocalRef<jobjectArray> j_argv =
      ToJavaArrayOfStrings(env, command_line()->argv());

  size_t file_count = files_to_register->GetMappingSize();
  DCHECK(file_count > 0);

  ScopedJavaLocalRef<jclass> j_file_info_class = base::android::GetClass(
      env, "org/chromium/base/process_launcher/FileDescriptorInfo");
  ScopedJavaLocalRef<jobjectArray> j_file_infos(
      env, env->NewObjectArray(file_count, j_file_info_class.obj(), NULL));
  base::android::CheckException(env);

  for (size_t i = 0; i < file_count; ++i) {
    int fd = files_to_register->GetFDAt(i);
    PCHECK(0 <= fd);
    int id = files_to_register->GetIDAt(i);
    const auto& region = files_to_register->GetRegionAt(i);
    bool auto_close = files_to_register->OwnsFD(fd);
    if (auto_close) {
      ignore_result(files_to_register->ReleaseFD(fd).release());
    }

    ScopedJavaLocalRef<jobject> j_file_info =
        Java_ChildProcessLauncherHelperImpl_makeFdInfo(
            env, id, fd, auto_close, region.offset, region.size);
    PCHECK(j_file_info.obj());
    env->SetObjectArrayElement(j_file_infos.obj(), i, j_file_info.obj());
  }

  java_peer_.Reset(Java_ChildProcessLauncherHelperImpl_createAndStart(
      env, reinterpret_cast<intptr_t>(this), j_argv, j_file_infos));
  AddRef();   
  base::PostTaskWithTraits(
      FROM_HERE, {client_thread_id_},
      base::BindOnce(
          &ChildProcessLauncherHelper::set_java_peer_available_on_client_thread,
          this));

  return Process();
}

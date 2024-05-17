static void JNI_ChildProcessLauncherHelperImpl_SetTerminationInfo(
    JNIEnv* env,
    jlong termination_info_ptr,
    jint binding_state,
    jboolean killed_by_us,
    jboolean clean_exit,
    jint remaining_process_with_strong_binding,
    jint remaining_process_with_moderate_binding,
    jint remaining_process_with_waived_binding) {
  ChildProcessTerminationInfo* info =
      reinterpret_cast<ChildProcessTerminationInfo*>(termination_info_ptr);
  info->binding_state =
      static_cast<base::android::ChildBindingState>(binding_state);
  info->was_killed_intentionally_by_browser = killed_by_us;
  info->clean_exit = clean_exit;
  info->remaining_process_with_strong_binding =
      remaining_process_with_strong_binding;
  info->remaining_process_with_moderate_binding =
      remaining_process_with_moderate_binding;
  info->remaining_process_with_waived_binding =
      remaining_process_with_waived_binding;
}

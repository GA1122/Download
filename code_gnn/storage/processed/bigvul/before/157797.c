bool WebContentsImpl::IsCrashed() const {
  switch (crashed_status_) {
    case base::TERMINATION_STATUS_PROCESS_CRASHED:
    case base::TERMINATION_STATUS_ABNORMAL_TERMINATION:
    case base::TERMINATION_STATUS_PROCESS_WAS_KILLED:
    case base::TERMINATION_STATUS_OOM:
    case base::TERMINATION_STATUS_LAUNCH_FAILED:
#if defined(OS_CHROMEOS)
    case base::TERMINATION_STATUS_PROCESS_WAS_KILLED_BY_OOM:
#endif
#if defined(OS_ANDROID)
    case base::TERMINATION_STATUS_OOM_PROTECTED:
#endif
      return true;
    case base::TERMINATION_STATUS_NORMAL_TERMINATION:
    case base::TERMINATION_STATUS_STILL_RUNNING:
      return false;
    case base::TERMINATION_STATUS_MAX_ENUM:
      NOTREACHED();
      return false;
  }

  NOTREACHED();
  return false;
}

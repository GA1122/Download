 DWORD UnprivilegedProcessDelegate::GetExitCode() {
  DCHECK(main_task_runner_->BelongsToCurrentThread());
// DWORD UnprivilegedProcessDelegate::GetProcessId() const {
//   if (worker_process_.IsValid()) {
//     return ::GetProcessId(worker_process_);
//   } else {
//     return 0;
//   }
// }
  
// bool UnprivilegedProcessDelegate::IsPermanentError(int failure_count) const {
//    
    DWORD exit_code = CONTROL_C_EXIT;
    if (worker_process_.IsValid()) {
      if (!::GetExitCodeProcess(worker_process_, &exit_code)) {
       LOG_GETLASTERROR(INFO)
           << "Failed to query the exit code of the worker process";
       exit_code = CONTROL_C_EXIT;
      }
    }
  
  return exit_code;
//    
//    
//   return (kMinPermanentErrorExitCode <= exit_code &&
//       exit_code <= kMaxPermanentErrorExitCode);
  }
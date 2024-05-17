DWORD StartRestrictedProcessInJob(wchar_t *command_line,
                                  TokenLevel primary_level,
                                  TokenLevel impersonation_level,
                                  JobLevel job_level,
                                  HANDLE *const job_handle_ret) {
  Job job;
  DWORD err_code = job.Init(job_level, NULL, 0);
  if (ERROR_SUCCESS != err_code)
    return err_code;

  if (JOB_UNPROTECTED != job_level) {
    err_code = job.UserHandleGrantAccess(GetDesktopWindow());
    if (ERROR_SUCCESS != err_code)
      return err_code;
  }

  HANDLE primary_token_handle = NULL;
  err_code = CreateRestrictedToken(&primary_token_handle,
                                   primary_level,
                                   INTEGRITY_LEVEL_LAST,
                                   PRIMARY);
  if (ERROR_SUCCESS != err_code) {
    return err_code;
  }
  base::win::ScopedHandle primary_token(primary_token_handle);

  HANDLE impersonation_token_handle;
  err_code = CreateRestrictedToken(&impersonation_token_handle,
                                   impersonation_level,
                                   INTEGRITY_LEVEL_LAST,
                                   IMPERSONATION);
  if (ERROR_SUCCESS != err_code) {
    return err_code;
  }
  base::win::ScopedHandle impersonation_token(impersonation_token_handle);

  STARTUPINFO startup_info = {0};
  base::win::ScopedProcessInformation process_info;

  if (!::CreateProcessAsUser(primary_token.Get(),
                             NULL,    
                             command_line,
                             NULL,    
                             NULL,    
                             FALSE,   
                             CREATE_SUSPENDED | CREATE_BREAKAWAY_FROM_JOB,
                             NULL,    
                             NULL,    
                             &startup_info,
                             process_info.Receive())) {
    return ::GetLastError();
  }

  {
    HANDLE temp_thread = process_info.thread_handle();
    if (!::SetThreadToken(&temp_thread, impersonation_token.Get())) {
      ::TerminateProcess(process_info.process_handle(),
                         0);   
      return ::GetLastError();
    }
  }

  err_code = job.AssignProcessToJob(process_info.process_handle());
  if (ERROR_SUCCESS != err_code) {
    ::TerminateProcess(process_info.process_handle(),
                       0);   
    return ::GetLastError();
  }

  ::ResumeThread(process_info.thread_handle());

  (*job_handle_ret) = job.Detach();

  return ERROR_SUCCESS;
}

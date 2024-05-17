bool NaClProcessHost::AttachDebugExceptionHandler(const std::string& info,
                                                  IPC::Message* reply_msg) {
  if (!enable_exception_handling_) {
    DLOG(ERROR) <<
        "Exception handling requested by NaCl process when not enabled";
    return false;
  }
  if (debug_exception_handler_requested_) {
    DLOG(ERROR) << "Multiple AttachDebugExceptionHandler requests received";
    return false;
  }
  debug_exception_handler_requested_ = true;

  base::ProcessId nacl_pid = base::GetProcId(process_->GetData().handle);
  base::win::ScopedHandle process_handle;
  if (!base::OpenProcessHandleWithAccess(
           nacl_pid,
           base::kProcessAccessQueryInformation |
           base::kProcessAccessSuspendResume |
           base::kProcessAccessTerminate |
           base::kProcessAccessVMOperation |
           base::kProcessAccessVMRead |
           base::kProcessAccessVMWrite |
           base::kProcessAccessWaitForTermination,
           process_handle.Receive())) {
    LOG(ERROR) << "Failed to get process handle";
    return false;
  }

  attach_debug_exception_handler_reply_msg_.reset(reply_msg);
  if (RunningOnWOW64()) {
    return NaClBrokerService::GetInstance()->LaunchDebugExceptionHandler(
               weak_factory_.GetWeakPtr(), nacl_pid, process_handle, info);
  } else {
    NaClStartDebugExceptionHandlerThread(
        process_handle.Take(), info,
        base::MessageLoopProxy::current(),
        base::Bind(&NaClProcessHost::OnDebugExceptionHandlerLaunchedByBroker,
                   weak_factory_.GetWeakPtr()));
    return true;
  }
}

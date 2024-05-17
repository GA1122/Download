bool CreateRemoteSessionProcess(
    uint32 session_id,
    const FilePath::StringType& application_name,
    const CommandLine::StringType& command_line,
    DWORD creation_flags,
    PROCESS_INFORMATION* process_information_out)
{
  DCHECK(base::win::GetVersion() == base::win::VERSION_XP);

  base::win::ScopedHandle pipe;
  if (!ConnectToExecutionServer(session_id, &pipe))
    return false;

  if (!SendCreateProcessRequest(pipe, application_name, command_line,
                                creation_flags)) {
    return false;
  }

  PROCESS_INFORMATION process_information;
  if (!ReceiveCreateProcessResponse(pipe, &process_information))
    return false;

  if (!ProcessCreateProcessResponse(creation_flags, &process_information)) {
    CloseHandlesAndTerminateProcess(&process_information);
    return false;
  }

  *process_information_out = process_information;
  return true;
}

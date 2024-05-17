bool LaunchProcessWithToken(const FilePath& binary,
                            const CommandLine::StringType& command_line,
                            HANDLE user_token,
                            bool inherit_handles,
                            DWORD creation_flags,
                            ScopedHandle* process_out,
                            ScopedHandle* thread_out) {
  FilePath::StringType application_name = binary.value();

  base::win::ScopedProcessInformation process_info;
  STARTUPINFOW startup_info;

  string16 desktop_name(UTF8ToUTF16(kDefaultDesktopName));

  memset(&startup_info, 0, sizeof(startup_info));
  startup_info.cb = sizeof(startup_info);
  startup_info.lpDesktop = const_cast<char16*>(desktop_name.c_str());

  BOOL result = CreateProcessAsUser(user_token,
                                    application_name.c_str(),
                                    const_cast<LPWSTR>(command_line.c_str()),
                                    NULL,
                                    NULL,
                                    inherit_handles,
                                    creation_flags,
                                    NULL,
                                    NULL,
                                    &startup_info,
                                    process_info.Receive());

  if (!result &&
      GetLastError() == ERROR_PIPE_NOT_CONNECTED &&
      base::win::GetVersion() == base::win::VERSION_XP) {
    DWORD session_id;
    DWORD return_length;
    result = GetTokenInformation(user_token,
                                 TokenSessionId,
                                 &session_id,
                                 sizeof(session_id),
                                 &return_length);
    if (result && session_id != 0) {
      result = CreateRemoteSessionProcess(session_id,
                                          application_name,
                                          command_line,
                                          creation_flags,
                                          process_info.Receive());
    } else {
      result = FALSE;
      SetLastError(ERROR_PIPE_NOT_CONNECTED);
    }
  }

  if (!result) {
    LOG_GETLASTERROR(ERROR) <<
        "Failed to launch a process with a user token";
    return false;
  }

  CHECK(process_info.IsValid());
  process_out->Set(process_info.TakeProcessHandle());
  thread_out->Set(process_info.TakeThreadHandle());
  return true;
}

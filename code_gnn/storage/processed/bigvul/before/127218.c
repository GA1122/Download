bool SendCreateProcessRequest(
    HANDLE pipe,
    const FilePath::StringType& application_name,
    const CommandLine::StringType& command_line,
    DWORD creation_flags) {
  string16 desktop_name(UTF8ToUTF16(kDefaultDesktopName));

  struct CreateProcessRequest {
    DWORD size;
    DWORD process_id;
    BOOL use_default_token;
    HANDLE token;
    LPWSTR application_name;
    LPWSTR command_line;
    SECURITY_ATTRIBUTES process_attributes;
    SECURITY_ATTRIBUTES thread_attributes;
    BOOL inherit_handles;
    DWORD creation_flags;
    LPVOID environment;
    LPWSTR current_directory;
    STARTUPINFOW startup_info;
    PROCESS_INFORMATION process_information;
  };

  size_t size = sizeof(CreateProcessRequest) + sizeof(wchar_t) *
      (application_name.size() + command_line.size() + desktop_name.size() + 3);
  scoped_array<char> buffer(new char[size]);
  memset(buffer.get(), 0, size);

  CreateProcessRequest* request =
      reinterpret_cast<CreateProcessRequest*>(buffer.get());
  request->size = size;
  request->process_id = GetCurrentProcessId();
  request->use_default_token = TRUE;
  request->creation_flags = creation_flags | CREATE_SUSPENDED;
  request->startup_info.cb = sizeof(request->startup_info);

  size_t buffer_offset = sizeof(CreateProcessRequest);

  request->application_name = reinterpret_cast<LPWSTR>(buffer_offset);
  std::copy(application_name.begin(),
            application_name.end(),
            reinterpret_cast<wchar_t*>(buffer.get() + buffer_offset));
  buffer_offset += (application_name.size() + 1) * sizeof(wchar_t);

  request->command_line = reinterpret_cast<LPWSTR>(buffer_offset);
  std::copy(command_line.begin(),
            command_line.end(),
            reinterpret_cast<wchar_t*>(buffer.get() + buffer_offset));
  buffer_offset += (command_line.size() + 1) * sizeof(wchar_t);

  request->startup_info.lpDesktop =
      reinterpret_cast<LPWSTR>(buffer_offset);
  std::copy(desktop_name.begin(),
            desktop_name.end(),
            reinterpret_cast<wchar_t*>(buffer.get() + buffer_offset));

  DWORD bytes;
  if (!WriteFile(pipe, buffer.get(), size, &bytes, NULL)) {
    LOG_GETLASTERROR(ERROR) << "Failed to send CreateProcessAsUser request";
    return false;
  }

  return true;
}

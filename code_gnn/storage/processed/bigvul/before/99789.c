int TestRunner::InternalRunTest(const wchar_t* command) {
  if (!is_init_)
    return SBOX_TEST_FAILED_TO_RUN_TEST;

  wchar_t prog_name[MAX_PATH];
  GetModuleFileNameW(NULL, prog_name, MAX_PATH);

  ResultCode result = SBOX_ALL_OK;
  PROCESS_INFORMATION target = {0};

  std::wstring arguments(L"\"");
  arguments += prog_name;
  arguments += L"\" -child ";
  arguments += command;

  result = broker_->SpawnTarget(prog_name, arguments.c_str(), policy_,
                                &target);

  if (SBOX_ALL_OK != result)
    return SBOX_TEST_FAILED_TO_RUN_TEST;

  ::ResumeThread(target.hThread);

  if (::IsDebuggerPresent()) {
    timeout_ = INFINITE;
  }

  if (WAIT_TIMEOUT == ::WaitForSingleObject(target.hProcess, timeout_)) {
    ::TerminateProcess(target.hProcess, SBOX_TEST_TIMED_OUT);
    ::CloseHandle(target.hProcess);
    ::CloseHandle(target.hThread);
    return SBOX_TEST_TIMED_OUT;
  }

  DWORD exit_code = SBOX_TEST_LAST_RESULT;
  if (!::GetExitCodeProcess(target.hProcess, &exit_code)) {
    ::CloseHandle(target.hProcess);
    ::CloseHandle(target.hThread);
    return SBOX_TEST_FAILED_TO_RUN_TEST;
  }

  ::CloseHandle(target.hProcess);
  ::CloseHandle(target.hThread);

  return exit_code;
}

int TestRunner::RunTest(const wchar_t* command) {
  if (MAX_STATE > 10)
    return SBOX_TEST_INVALID_PARAMETER;

  wchar_t state_number[2];
  state_number[0] = L'0' + state_;
  state_number[1] = L'\0';
  std::wstring full_command(state_number);
  full_command += L" ";
  full_command += command;

  return InternalRunTest(full_command.c_str());
}

int DispatchCall(int argc, wchar_t **argv) {
  if (argc < 4)
    return SBOX_TEST_INVALID_PARAMETER;

  if (0 == _wcsicmp(argv[3], L"wait")) {
      Sleep(INFINITE);
      return SBOX_TEST_TIMED_OUT;
  }

  if (0 == _wcsicmp(argv[3], L"ping"))
      return SBOX_TEST_PING_OK;

  SboxTestsState state = static_cast<SboxTestsState>(_wtoi(argv[2]));
  if ((state <= MIN_STATE) || (state >= MAX_STATE))
    return SBOX_TEST_INVALID_PARAMETER;

  HMODULE module;
  if (!GetModuleHandleHelper(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
                                 GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                             reinterpret_cast<wchar_t*>(&DispatchCall),
                             &module))
    return SBOX_TEST_FAILED_TO_EXECUTE_COMMAND;

  std::string command_name = WideToMultiByte(argv[3]);
  CommandFunction command = reinterpret_cast<CommandFunction>(
                                ::GetProcAddress(module, command_name.c_str()));
  if (!command)
    return SBOX_TEST_FAILED_TO_EXECUTE_COMMAND;

  if (BEFORE_INIT == state)
    return command(argc - 4, argv + 4);
  else if (EVERY_STATE == state)
    command(argc - 4, argv + 4);

  TargetServices* target = SandboxFactory::GetTargetServices();
  if (!target)
    return SBOX_TEST_FAILED_TO_EXECUTE_COMMAND;

  if (SBOX_ALL_OK != target->Init())
    return SBOX_TEST_FAILED_TO_EXECUTE_COMMAND;

  if (BEFORE_REVERT == state)
    return command(argc - 4, argv + 4);
  else if (EVERY_STATE == state)
    command(argc - 4, argv + 4);

  target->LowerToken();
  return command(argc - 4, argv + 4);
}

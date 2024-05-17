int TestOpenWriteFile(int argc, wchar_t **argv) {
  if (1 != argc)
    return SBOX_TEST_FAILED_TO_EXECUTE_COMMAND;

  std::wstring path = argv[0];
  trim_quote(&path);

  return TestOpenWriteFile(path);
  }

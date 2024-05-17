void ShowPackExtensionMessage(const std::wstring caption,
    const std::wstring message) {
#if defined(OS_WIN)
  win_util::MessageBox(NULL, message, caption, MB_OK | MB_SETFOREGROUND);
#else
  std::string out_text = WideToASCII(caption);
  out_text.append("\n\n");
  out_text.append(WideToASCII(message));
  out_text.append("\n");
  printf("%s", out_text.c_str());
#endif
}

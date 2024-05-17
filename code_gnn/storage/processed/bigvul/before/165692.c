std::vector<std::wstring> TokenizeCommandLineToArray(
    const std::wstring& command_line) {
  enum class SpecialChars {
    kInterpret,

    kIgnore,
  } state;

  static constexpr wchar_t kSpaceTab[] = L" \t";

  std::vector<std::wstring> result;
  const wchar_t* p = command_line.c_str();

  size_t argv0_length = 0;
  if (p[0] == L'"') {
    const wchar_t* closing = wcschr(++p, L'"');
    if (!closing)
      argv0_length = command_line.size() - 1;   
    else
      argv0_length = closing - (command_line.c_str() + 1);
  } else {
    argv0_length = wcscspn(p, kSpaceTab);
  }
  result.emplace_back(p, argv0_length);
  if (p[argv0_length] == 0)
    return result;
  p += argv0_length + 1;

  std::wstring token;
  for (;;) {
    p += wcsspn(p, kSpaceTab);

    if (p[0] == 0)
      break;

    state = SpecialChars::kInterpret;

    for (;;) {
      int num_backslashes = wcsspn(p, L"\\");
      p += num_backslashes;

      if (p[0] == L'"') {
        token.append(num_backslashes / 2, L'\\');

        if (num_backslashes % 2 == 1) {
          token += L'"';
        } else if (p[1] == L'"' && state == SpecialChars::kIgnore) {
          ++p;
          token += L'"';
          state = SpecialChars::kInterpret;
        } else {
          state = state == SpecialChars::kInterpret ? SpecialChars::kIgnore
                                                    : SpecialChars::kInterpret;
        }
      } else {
        token.append(num_backslashes, L'\\');
        if (p[0] == 0 ||
            (state == SpecialChars::kInterpret && wcschr(kSpaceTab, p[0]))) {
          result.push_back(token);
          token.clear();
          break;
        }

        token += *p;
      }

      ++p;
    }
  }

  return result;
}

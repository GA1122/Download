void FormatStringForOS(base::string16* text) {
#if defined(OS_POSIX)
  static const base::char16 kCr[] = {L'\r', L'\0'};
  static const base::char16 kBlank[] = {L'\0'};
  base::ReplaceChars(*text, kCr, kBlank, text);
#elif defined(OS_WIN)
#else
  NOTIMPLEMENTED();
#endif
}

string16 PrependWindowsSessionPath(const char16* object) {
  static uintptr_t s_session_id = 0;
  if (s_session_id == 0) {
    HANDLE token;
    DWORD session_id_length;
    DWORD session_id = 0;

    CHECK(::OpenProcessToken(::GetCurrentProcess(), TOKEN_QUERY, &token));
    CHECK(::GetTokenInformation(token, TokenSessionId, &session_id,
        sizeof(session_id), &session_id_length));
    CloseHandle(token);
    if (session_id)
      s_session_id = session_id;
  }

  return base::StringPrintf(L"\\Sessions\\%d%ls", s_session_id, object);
}

bool IsInteractiveDesktop(bool* is_interactive) {
  HDESK current_desk = ::GetThreadDesktop(::GetCurrentThreadId());
  if (NULL == current_desk) {
    return false;
  }
  wchar_t current_desk_name[256] = {0};
  if (!::GetUserObjectInformationW(current_desk, UOI_NAME, current_desk_name,
                                  sizeof(current_desk_name), NULL)) {
    return false;
  }
  *is_interactive = (0 == _wcsicmp(L"default", current_desk_name));
  return true;
}

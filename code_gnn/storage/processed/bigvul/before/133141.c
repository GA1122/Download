LRESULT HWNDMessageHandler::OnSetIcon(UINT size_type, HICON new_icon) {
  return DefWindowProcWithRedrawLock(WM_SETICON, size_type,
                                     reinterpret_cast<LPARAM>(new_icon));
}

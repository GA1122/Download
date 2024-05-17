LRESULT HWNDMessageHandler::OnSetText(const wchar_t* text) {
  return DefWindowProcWithRedrawLock(WM_SETTEXT, NULL,
                                     reinterpret_cast<LPARAM>(text));
}

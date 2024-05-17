WindowManagerName GuessWindowManager() {
  std::string name;
  if (GetWindowManagerName(&name)) {
    if (name == "Compiz" || name == "compiz")
      return WM_COMPIZ;
    if (name == "KWin")
      return WM_KWIN;
    if (name == "Metacity")
      return WM_METACITY;
    if (name == "Mutter")
      return WM_MUTTER;
    if (name == "Xfwm4")
      return WM_XFWM4;
    if (name == "chromeos-wm")
      return WM_CHROME_OS;
    if (name == "Blackbox")
      return WM_BLACKBOX;
    if (name == "e16")
      return WM_ENLIGHTENMENT;
    if (StartsWithASCII(name, "IceWM", true))
      return WM_ICE_WM;
    if (name == "Openbox")
      return WM_OPENBOX;
  }
  return WM_UNKNOWN;
}

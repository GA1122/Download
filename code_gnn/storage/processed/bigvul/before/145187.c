  bool UseOpenGLRenderer() {
    return cmd_line_.GetSwitchValueASCII(switches::kUseGL) ==
           gl::kGLImplementationDesktopName;
  }

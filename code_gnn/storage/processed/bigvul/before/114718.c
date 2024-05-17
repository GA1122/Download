  virtual void SetUpCommandLine(CommandLine* command_line) {
    command_line->AppendSwitchASCII(switches::kJavaScriptFlags, "--expose-gc");
  }

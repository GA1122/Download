  ~SandboxSymbolizeHelper() {
    UnregisterCallback();
    CloseObjectFiles();
  }
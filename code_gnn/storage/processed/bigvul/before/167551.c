  bool IsProcessBackgrounded(const base::Process& process) {
#if defined(OS_MACOSX)
    return process.IsProcessBackgrounded(port_provider_);
#else
    return process.IsProcessBackgrounded();
#endif
  }

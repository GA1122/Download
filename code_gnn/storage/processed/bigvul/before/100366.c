  virtual void CleanUp() {
    delete render_process_;

#if defined(OS_WIN)
    CoUninitialize();
#endif
  }

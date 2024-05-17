  void DidGetFileInfo(File::Error error,
                      const File::Info& file_info) {
    error_ = error;
    file_info_ = file_info;
    MessageLoop::current()->QuitWhenIdle();
  }

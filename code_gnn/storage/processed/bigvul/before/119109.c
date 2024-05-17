  void DidCreateTemporary(File::Error error,
                          const FilePath& path) {
    error_ = error;
    path_ = path;
    MessageLoop::current()->QuitWhenIdle();
  }

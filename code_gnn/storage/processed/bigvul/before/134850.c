  virtual GURL TestUrl() {
    return net::FilePathToFileURL(
        test_dir().AppendASCII("rwhv_compositing_animation.html"));
  }

  virtual void CleanUp() {
    render_process_.reset();

    SetThreadWasQuitProperly(true);
  }

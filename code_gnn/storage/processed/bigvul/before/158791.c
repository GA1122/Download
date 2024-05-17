  void Wait() {
    if (!found_value_)
      run_loop_.Run();
  }

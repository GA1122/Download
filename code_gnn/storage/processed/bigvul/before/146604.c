  ~ScopedUnpackParametersResetRestore() {
    if (enabled_)
      context_->RestoreUnpackParameters();
  }

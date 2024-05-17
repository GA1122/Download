    void ShutDown() {
      if (!is_active_)
        return;

      is_active_ = false;
      if (context_)
        context_->ShutDownOnIOThread();

      delete this;
    }

    void ActivateAndWait() {
      widget_->Activate();
      if (!widget_->IsActive()) {
        waiting_ = true;
        content::RunMessageLoop();
      }
    }

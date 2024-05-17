  ~TaskManagerTableModel() {
    model_->RemoveObserver(this);
  }

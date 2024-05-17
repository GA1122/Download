  virtual void Run() {
    MessageLoop::current()->PostTask(FROM_HERE, task_);
  }

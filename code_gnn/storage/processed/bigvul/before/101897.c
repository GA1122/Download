  virtual void Run() {
    MessageLoop::current()->PostTask(FROM_HERE, new CloseWindowTask(browser_));
    browser_->ShowAppMenu();
  }

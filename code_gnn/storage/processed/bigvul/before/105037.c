  virtual void Start() {
    MessageLoop::current()->PostTask(
        FROM_HERE,
        method_factory_.NewRunnableMethod(&FakeURLFetcher::RunDelegate));
  }

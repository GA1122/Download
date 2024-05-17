  virtual void onContextLost() {
    MessageLoop::current()->PostTask(
        FROM_HERE,
        base::Bind(&GpuProcessTransportFactory::OnLostSharedContext,
                   callback_factory_.GetWeakPtr()));
  }

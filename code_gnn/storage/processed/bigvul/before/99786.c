 BrokerServices* GetBroker() {
   static BrokerServices* broker = SandboxFactory::GetBrokerServices();
   static bool is_initialized = false;

  if (!broker) {
    return NULL;
  }

  if (!is_initialized) {
    if (SBOX_ALL_OK != broker->Init())
      return NULL;

    is_initialized = true;
  }

  return broker;
}

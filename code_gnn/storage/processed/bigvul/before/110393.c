void PluginModule::SetBroker(PluginDelegate::Broker* broker) {
  DCHECK(!broker_ || !broker);
  broker_ = broker;
}

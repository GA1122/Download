void SecurityHandler::Wire(UberDispatcher* dispatcher) {
  frontend_.reset(new Security::Frontend(dispatcher->channel()));
  Security::Dispatcher::wire(dispatcher, this);
}

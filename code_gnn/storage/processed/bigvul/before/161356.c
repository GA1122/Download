void NetworkHandler::Wire(UberDispatcher* dispatcher) {
  frontend_.reset(new Network::Frontend(dispatcher->channel()));
   Network::Dispatcher::wire(dispatcher, this);
 }

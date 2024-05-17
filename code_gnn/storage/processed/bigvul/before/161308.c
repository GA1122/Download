void IOHandler::Wire(UberDispatcher* dispatcher) {
  frontend_.reset(new IO::Frontend(dispatcher->channel()));
   IO::Dispatcher::wire(dispatcher, this);
 }

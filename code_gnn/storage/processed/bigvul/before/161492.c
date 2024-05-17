void TargetHandler::Wire(UberDispatcher* dispatcher) {
  frontend_.reset(new Target::Frontend(dispatcher->channel()));
   Target::Dispatcher::wire(dispatcher, this);
 }

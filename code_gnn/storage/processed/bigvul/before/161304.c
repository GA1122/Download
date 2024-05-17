void InspectorHandler::Wire(UberDispatcher* dispatcher) {
  frontend_.reset(new Inspector::Frontend(dispatcher->channel()));
   Inspector::Dispatcher::wire(dispatcher, this);
 }

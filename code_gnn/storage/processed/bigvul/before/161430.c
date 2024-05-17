void ServiceWorkerHandler::Wire(UberDispatcher* dispatcher) {
  frontend_.reset(new ServiceWorker::Frontend(dispatcher->channel()));
   ServiceWorker::Dispatcher::wire(dispatcher, this);
 }

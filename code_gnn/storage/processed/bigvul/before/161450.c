void StorageHandler::Wire(UberDispatcher* dispatcher) {
  frontend_ = std::make_unique<Storage::Frontend>(dispatcher->channel());
   Storage::Dispatcher::wire(dispatcher, this);
 }

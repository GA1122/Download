void InputHandler::Wire(UberDispatcher* dispatcher) {
  Input::Dispatcher::wire(dispatcher, this);
}

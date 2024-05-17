InputDispatcherThread::InputDispatcherThread(const sp<InputDispatcherInterface>& dispatcher) :
 Thread(  true), mDispatcher(dispatcher) {
}

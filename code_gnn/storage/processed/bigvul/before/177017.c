bool InputDispatcherThread::threadLoop() {
    mDispatcher->dispatchOnce();
 return true;
}

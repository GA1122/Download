void InputDispatcher::dump(String8& dump) {
 AutoMutex _l(mLock);

    dump.append("Input Dispatcher State:\n");
    dumpDispatchStateLocked(dump);

 if (!mLastANRState.isEmpty()) {
        dump.append("\nInput Dispatcher State at time of last ANR:\n");
        dump.append(mLastANRState);
 }
}

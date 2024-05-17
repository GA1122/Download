void InputDispatcher::dispatchEventLocked(nsecs_t currentTime,
 EventEntry* eventEntry, const Vector<InputTarget>& inputTargets) {
#if DEBUG_DISPATCH_CYCLE
    ALOGD("dispatchEventToCurrentInputTargets");
#endif

    ALOG_ASSERT(eventEntry->dispatchInProgress);  

    pokeUserActivityLocked(eventEntry);

 for (size_t i = 0; i < inputTargets.size(); i++) {
 const InputTarget& inputTarget = inputTargets.itemAt(i);

 ssize_t connectionIndex = getConnectionIndexLocked(inputTarget.inputChannel);
 if (connectionIndex >= 0) {
            sp<Connection> connection = mConnectionsByFd.valueAt(connectionIndex);
            prepareDispatchCycleLocked(currentTime, connection, eventEntry, &inputTarget);
 } else {
#if DEBUG_FOCUS
            ALOGD("Dropping event delivery to target with channel '%s' because it "
 "is no longer registered with the input dispatcher.",
                    inputTarget.inputChannel->getName().string());
#endif
 }
 }
}

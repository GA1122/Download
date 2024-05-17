void InputDispatcher::enqueueDispatchEntriesLocked(nsecs_t currentTime,
 const sp<Connection>& connection, EventEntry* eventEntry, const InputTarget* inputTarget) {
 bool wasEmpty = connection->outboundQueue.isEmpty();

    enqueueDispatchEntryLocked(connection, eventEntry, inputTarget,
 InputTarget::FLAG_DISPATCH_AS_HOVER_EXIT);
    enqueueDispatchEntryLocked(connection, eventEntry, inputTarget,
 InputTarget::FLAG_DISPATCH_AS_OUTSIDE);
    enqueueDispatchEntryLocked(connection, eventEntry, inputTarget,
 InputTarget::FLAG_DISPATCH_AS_HOVER_ENTER);
    enqueueDispatchEntryLocked(connection, eventEntry, inputTarget,
 InputTarget::FLAG_DISPATCH_AS_IS);
    enqueueDispatchEntryLocked(connection, eventEntry, inputTarget,
 InputTarget::FLAG_DISPATCH_AS_SLIPPERY_EXIT);
    enqueueDispatchEntryLocked(connection, eventEntry, inputTarget,
 InputTarget::FLAG_DISPATCH_AS_SLIPPERY_ENTER);

 if (wasEmpty && !connection->outboundQueue.isEmpty()) {
        startDispatchCycleLocked(currentTime, connection);
 }
}

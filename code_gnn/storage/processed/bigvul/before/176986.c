void InputDispatcher::releaseDispatchEntryLocked(DispatchEntry* dispatchEntry) {
 if (dispatchEntry->hasForegroundTarget()) {
        decrementPendingForegroundDispatchesLocked(dispatchEntry->eventEntry);
 }
 delete dispatchEntry;
}

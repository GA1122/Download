void InputDispatcher::decrementPendingForegroundDispatchesLocked(EventEntry* entry) {
 InjectionState* injectionState = entry->injectionState;
 if (injectionState) {
        injectionState->pendingForegroundDispatches -= 1;

 if (injectionState->pendingForegroundDispatches == 0) {
            mInjectionSyncFinishedCondition.broadcast();
 }
 }
}

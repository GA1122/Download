void InputDispatcher::incrementPendingForegroundDispatchesLocked(EventEntry* entry) {
 InjectionState* injectionState = entry->injectionState;
 if (injectionState) {
        injectionState->pendingForegroundDispatches += 1;
 }
}

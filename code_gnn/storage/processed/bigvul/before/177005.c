bool InputDispatcher::InputState::shouldCancelKey(const KeyMemento& memento,
 const CancelationOptions& options) {
 if (options.keyCode != -1 && memento.keyCode != options.keyCode) {
 return false;
 }

 if (options.deviceId != -1 && memento.deviceId != options.deviceId) {
 return false;
 }

 switch (options.mode) {
 case CancelationOptions::CANCEL_ALL_EVENTS:
 case CancelationOptions::CANCEL_NON_POINTER_EVENTS:
 return true;
 case CancelationOptions::CANCEL_FALLBACK_EVENTS:
 return memento.flags & AKEY_EVENT_FLAG_FALLBACK;
 default:
 return false;
 }
}

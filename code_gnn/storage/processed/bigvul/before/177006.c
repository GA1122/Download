bool InputDispatcher::InputState::shouldCancelMotion(const MotionMemento& memento,
 const CancelationOptions& options) {
 if (options.deviceId != -1 && memento.deviceId != options.deviceId) {
 return false;
 }

 switch (options.mode) {
 case CancelationOptions::CANCEL_ALL_EVENTS:
 return true;
 case CancelationOptions::CANCEL_POINTER_EVENTS:
 return memento.source & AINPUT_SOURCE_CLASS_POINTER;
 case CancelationOptions::CANCEL_NON_POINTER_EVENTS:
 return !(memento.source & AINPUT_SOURCE_CLASS_POINTER);
 default:
 return false;
 }
}

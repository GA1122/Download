nsecs_t InputDispatcher::getTimeSpentWaitingForApplicationLocked(
 nsecs_t currentTime) {
 if (mInputTargetWaitCause == INPUT_TARGET_WAIT_CAUSE_APPLICATION_NOT_READY) {
 return currentTime - mInputTargetWaitStartTime;
 }
 return 0;
}

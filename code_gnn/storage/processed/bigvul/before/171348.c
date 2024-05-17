bool OMXCodec::isIntermediateState(State state) {
 return state == LOADED_TO_IDLE
 || state == IDLE_TO_EXECUTING
 || state == EXECUTING_TO_IDLE
 || state == IDLE_TO_LOADED
 || state == RECONFIGURING;
}

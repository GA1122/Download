callStateToString(RIL_CallState s) {
 switch(s) {
 case RIL_CALL_ACTIVE : return "ACTIVE";
 case RIL_CALL_HOLDING: return "HOLDING";
 case RIL_CALL_DIALING: return "DIALING";
 case RIL_CALL_ALERTING: return "ALERTING";
 case RIL_CALL_INCOMING: return "INCOMING";
 case RIL_CALL_WAITING: return "WAITING";
 default: return "<unknown state>";
 }
}

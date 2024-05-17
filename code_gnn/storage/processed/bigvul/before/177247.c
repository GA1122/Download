static inline status_t makeNoSideEffectStatus(status_t err) {
 switch (err) {
 case INVALID_OPERATION:
 case DEAD_OBJECT:
 return UNKNOWN_ERROR;
 default:
 return err;
 }
}

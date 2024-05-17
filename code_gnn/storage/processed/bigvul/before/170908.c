static inline const char *portString(OMX_U32 portIndex) {
 switch (portIndex) {
 case kPortIndexInput: return "Input";
 case kPortIndexOutput: return "Output";
 case ~0U: return "All";
 default: return "port";
 }
}

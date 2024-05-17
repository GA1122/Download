const char *ACodec::_asString(BufferInfo::Status s) {
 switch (s) {
 case BufferInfo::OWNED_BY_US: return "OUR";
 case BufferInfo::OWNED_BY_COMPONENT: return "COMPONENT";
 case BufferInfo::OWNED_BY_UPSTREAM: return "UPSTREAM";
 case BufferInfo::OWNED_BY_DOWNSTREAM: return "DOWNSTREAM";
 case BufferInfo::OWNED_BY_NATIVE_WINDOW: return "SURFACE";
 case BufferInfo::UNRECOGNIZED: return "UNRECOGNIZED";
 default: return "?";
 }
}

status_t MediaHTTP::reconnectAtOffset(off64_t offset) {
 return connect(mLastURI.c_str(), &mLastHeaders, offset);
}

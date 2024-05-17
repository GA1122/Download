String8 MediaHTTP::getUri() {
 if (mInitCheck != OK) {
 return String8::empty();
 }

 String8 uri;
 if (OK == mHTTPConnection->getUri(&uri)) {
 return uri;
 }
 return String8(mLastURI.c_str());
}

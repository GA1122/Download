 status_t MediaHTTP::connect(
  const char *uri,
  const KeyedVector<String8, String8> *headers,
  off64_t  ) {
  if (mInitCheck != OK) {
  return mInitCheck;
  }
 
  KeyedVector<String8, String8> extHeaders;
  if (headers != NULL) {
         extHeaders = *headers;
  }
 
  if (extHeaders.indexOfKey(String8("User-Agent")) < 0) {
 
          extHeaders.add(String8("User-Agent"), String8(MakeUserAgent().c_str()));
      }
  
    bool success = mHTTPConnection->connect(uri, &extHeaders);
//     mLastURI = uri;
//      
//      
//     uri = NULL;
// 
//     bool success = mHTTPConnection->connect(mLastURI.c_str(), &extHeaders);
  
      mLastHeaders = extHeaders;
    mLastURI = uri;
  
      mCachedSizeValid = false;
  
      if (success) {
        AString sanitized = uriDebugString(uri);
//         AString sanitized = uriDebugString(mLastURI);
          mName = String8::format("MediaHTTP(%s)", sanitized.c_str());
      }
  
  return success ? OK : UNKNOWN_ERROR;
 }
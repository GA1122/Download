MediaHTTP::MediaHTTP(const sp<IMediaHTTPConnection> &conn)
 : mInitCheck((conn != NULL) ? OK : NO_INIT),
      mHTTPConnection(conn),
      mCachedSizeValid(false),
      mCachedSize(0ll),
      mDrmManagerClient(NULL) {
}

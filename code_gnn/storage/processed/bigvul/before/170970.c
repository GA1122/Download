status_t MediaHTTP::getSize(off64_t *size) {
 if (mInitCheck != OK) {
 return mInitCheck;
 }


 if (!mCachedSizeValid) {
        mCachedSize = mHTTPConnection->getSize();
        mCachedSizeValid = true;
 }

 *size = mCachedSize;

 return *size < 0 ? *size : static_cast<status_t>(OK);
}

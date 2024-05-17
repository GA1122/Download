void MediaHTTP::disconnect() {
    mName = String8("MediaHTTP(<disconnected>)");
 if (mInitCheck != OK) {
 return;
 }

    mHTTPConnection->disconnect();
}

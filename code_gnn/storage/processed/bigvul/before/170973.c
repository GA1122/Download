ssize_t MediaHTTP::readAt(off64_t offset, void *data, size_t size) {
 if (mInitCheck != OK) {
 return mInitCheck;
 }

 int64_t startTimeUs = ALooper::GetNowUs();

 size_t numBytesRead = 0;
 while (numBytesRead < size) {
 size_t copy = size - numBytesRead;

 if (copy > 64 * 1024) {
            copy = 64 * 1024;
 }

 ssize_t n = mHTTPConnection->readAt(
                offset + numBytesRead, (uint8_t *)data + numBytesRead, copy);

 if (n < 0) {
 return n;
 } else if (n == 0) {
 break;
 }

        numBytesRead += n;
 }

 int64_t delayUs = ALooper::GetNowUs() - startTimeUs;

    addBandwidthMeasurement(numBytesRead, delayUs);

 return numBytesRead;
}

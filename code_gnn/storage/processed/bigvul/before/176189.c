void CodecHandler::onMessageReceived(const sp<AMessage> &msg) {

 switch (msg->what()) {
 case kWhatRequestActivityNotifications:
 {
 if (mCodec->mRequestedActivityNotification) {
 break;
 }

            mCodec->mCodec->requestActivityNotification(mCodec->mActivityNotification);
            mCodec->mRequestedActivityNotification = true;
 break;
 }

 case kWhatActivityNotify:
 {
 {
 int32_t generation;
                msg->findInt32("generation", &generation);

 if (generation != mCodec->mGeneration) {
 break;
 }

                mCodec->mRequestedActivityNotification = false;
 }

 if (mCodec->mCallback) {
                mCodec->mCallback(mCodec, mCodec->mCallbackUserData);
 }
 break;
 }

 case kWhatAsyncNotify:
 {
 int32_t cbID;
 if (!msg->findInt32("callbackID", &cbID)) {
                 ALOGE("kWhatAsyncNotify: callbackID is expected.");
 break;
 }

             ALOGV("kWhatAsyncNotify: cbID = %d", cbID);

 switch (cbID) {
 case MediaCodec::CB_INPUT_AVAILABLE:
 {
 int32_t index;
 if (!msg->findInt32("index", &index)) {
                         ALOGE("CB_INPUT_AVAILABLE: index is expected.");
 break;
 }

 Mutex::Autolock _l(mCodec->mAsyncCallbackLock);
 if (mCodec->mAsyncCallbackUserData != NULL
 || mCodec->mAsyncCallback.onAsyncInputAvailable != NULL) {
                         mCodec->mAsyncCallback.onAsyncInputAvailable(
                                 mCodec,
                                 mCodec->mAsyncCallbackUserData,
                                 index);
 }

 break;
 }

 case MediaCodec::CB_OUTPUT_AVAILABLE:
 {
 int32_t index;
 size_t offset;
 size_t size;
 int64_t timeUs;
 int32_t flags;

 if (!msg->findInt32("index", &index)) {
                         ALOGE("CB_OUTPUT_AVAILABLE: index is expected.");
 break;
 }
 if (!msg->findSize("offset", &offset)) {
                         ALOGE("CB_OUTPUT_AVAILABLE: offset is expected.");
 break;
 }
 if (!msg->findSize("size", &size)) {
                         ALOGE("CB_OUTPUT_AVAILABLE: size is expected.");
 break;
 }
 if (!msg->findInt64("timeUs", &timeUs)) {
                         ALOGE("CB_OUTPUT_AVAILABLE: timeUs is expected.");
 break;
 }
 if (!msg->findInt32("flags", &flags)) {
                         ALOGE("CB_OUTPUT_AVAILABLE: flags is expected.");
 break;
 }

 AMediaCodecBufferInfo bufferInfo = {
 (int32_t)offset,
 (int32_t)size,
                         timeUs,
 (uint32_t)flags};

 Mutex::Autolock _l(mCodec->mAsyncCallbackLock);
 if (mCodec->mAsyncCallbackUserData != NULL
 || mCodec->mAsyncCallback.onAsyncOutputAvailable != NULL) {
                         mCodec->mAsyncCallback.onAsyncOutputAvailable(
                                 mCodec,
                                 mCodec->mAsyncCallbackUserData,
                                 index,
 &bufferInfo);
 }

 break;
 }

 case MediaCodec::CB_OUTPUT_FORMAT_CHANGED:
 {
                     sp<AMessage> format;
 if (!msg->findMessage("format", &format)) {
                         ALOGE("CB_OUTPUT_FORMAT_CHANGED: format is expected.");
 break;
 }

 AMediaFormat *aMediaFormat = AMediaFormat_fromMsg(&format);

 Mutex::Autolock _l(mCodec->mAsyncCallbackLock);
 if (mCodec->mAsyncCallbackUserData != NULL
 || mCodec->mAsyncCallback.onAsyncFormatChanged != NULL) {
                         mCodec->mAsyncCallback.onAsyncFormatChanged(
                                 mCodec,
                                 mCodec->mAsyncCallbackUserData,
                                 aMediaFormat);
 }

 break;
 }

 case MediaCodec::CB_ERROR:
 {
 status_t err;
 int32_t actionCode;
 AString detail;
 if (!msg->findInt32("err", &err)) {
                         ALOGE("CB_ERROR: err is expected.");
 break;
 }
 if (!msg->findInt32("action", &actionCode)) {
                         ALOGE("CB_ERROR: action is expected.");
 break;
 }
                     msg->findString("detail", &detail);
                     ALOGE("Decoder reported error(0x%x), actionCode(%d), detail(%s)",
                           err, actionCode, detail.c_str());

 Mutex::Autolock _l(mCodec->mAsyncCallbackLock);
 if (mCodec->mAsyncCallbackUserData != NULL
 || mCodec->mAsyncCallback.onAsyncError != NULL) {
                         mCodec->mAsyncCallback.onAsyncError(
                                 mCodec,
                                 mCodec->mAsyncCallbackUserData,
                                 translate_error(err),
                                 actionCode,
                                 detail.c_str());
 }

 break;
 }

 default:
 {
                     ALOGE("kWhatAsyncNotify: callbackID(%d) is unexpected.", cbID);
 break;
 }
 }
 break;
 }

 case kWhatStopActivityNotifications:
 {
            sp<AReplyToken> replyID;
            msg->senderAwaitsResponse(&replyID);

            mCodec->mGeneration++;
            mCodec->mRequestedActivityNotification = false;

            sp<AMessage> response = new AMessage;
            response->postReply(replyID);
 break;
 }

 default:
            ALOGE("shouldn't be here");
 break;
 }

}

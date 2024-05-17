 virtual void onMessages(const std::list<omx_message> &messages) {
        sp<OMXCodec> codec = mTarget.promote();

 if (codec.get() != NULL) {
 Mutex::Autolock autoLock(codec->mLock);
 for (std::list<omx_message>::const_iterator it = messages.cbegin();
                  it != messages.cend(); ++it) {
                codec->on_message(*it);
 }
            codec.clear();
 }
 }

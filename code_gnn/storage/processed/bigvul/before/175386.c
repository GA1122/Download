static bool Downmix_validChannelMask(uint32_t mask)
{
 if (!mask) {
 return false;
 }
 if (mask & kUnsupported) {
        ALOGE("Unsupported channels (top or front left/right of center)");
 return false;
 }
 if ((mask & AUDIO_CHANNEL_OUT_STEREO) != AUDIO_CHANNEL_OUT_STEREO) {
        ALOGE("Front channels must be present");
 return false;
 }
 if ((mask & kSides) != 0) {
 if ((mask & kSides) != kSides) {
            ALOGE("Side channels must be used as a pair");
 return false;
 }
 }
 if ((mask & kBacks) != 0) {
 if ((mask & kBacks) != kBacks) {
            ALOGE("Back channels must be used as a pair");
 return false;
 }
 }
 return true;
}

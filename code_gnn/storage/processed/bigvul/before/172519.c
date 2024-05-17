static bool isVideoSizeSupported(
 int32_t width, int32_t height,
 const Vector<Size>& supportedSizes) {

    ALOGV("isVideoSizeSupported");
 for (size_t i = 0; i < supportedSizes.size(); ++i) {
 if (width  == supportedSizes[i].width &&
            height == supportedSizes[i].height) {
 return true;
 }
 }
 return false;
}

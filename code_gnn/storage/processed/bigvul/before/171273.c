android::SoftOMXComponent *createSoftOMXComponent(
 const char *name, const OMX_CALLBACKTYPE *callbacks,
        OMX_PTR appData, OMX_COMPONENTTYPE **component) {
 using namespace android;
 if (!strcmp(name, "OMX.google.h263.decoder")) {
 return new android::SoftMPEG4(
                name, "video_decoder.h263", OMX_VIDEO_CodingH263,
                kH263ProfileLevels, ARRAY_SIZE(kH263ProfileLevels),
                callbacks, appData, component);
 } else if (!strcmp(name, "OMX.google.mpeg4.decoder")) {
 return new android::SoftMPEG4(
                name, "video_decoder.mpeg4", OMX_VIDEO_CodingMPEG4,
                kM4VProfileLevels, ARRAY_SIZE(kM4VProfileLevels),
                callbacks, appData, component);
 } else {
        CHECK(!"Unknown component");
 }
 return NULL;
}

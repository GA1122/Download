android::SoftOMXComponent *createSoftOMXComponent(
 const char *name, const OMX_CALLBACKTYPE *callbacks,
        OMX_PTR appData, OMX_COMPONENTTYPE **component) {
 using namespace android;
 if (!strcmp(name, "OMX.google.h263.encoder")) {
 return new android::SoftMPEG4Encoder(
                name, "video_encoder.h263", OMX_VIDEO_CodingH263, MEDIA_MIMETYPE_VIDEO_H263,
                kH263ProfileLevels, NELEM(kH263ProfileLevels),
                callbacks, appData, component);
 } else if (!strcmp(name, "OMX.google.mpeg4.encoder")) {
 return new android::SoftMPEG4Encoder(
                name, "video_encoder.mpeg4", OMX_VIDEO_CodingMPEG4, MEDIA_MIMETYPE_VIDEO_MPEG4,
                kMPEG4ProfileLevels, NELEM(kMPEG4ProfileLevels),
                callbacks, appData, component);
 } else {
        CHECK(!"Unknown component");
 }
 return NULL;
}

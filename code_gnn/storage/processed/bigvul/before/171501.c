android::SoftOMXComponent *createSoftOMXComponent(
 const char *name, const OMX_CALLBACKTYPE *callbacks,
        OMX_PTR appData, OMX_COMPONENTTYPE **component) {
 if (!strcmp(name, "OMX.google.vp8.decoder")) {
 return new android::SoftVPX(
                name, "video_decoder.vp8", OMX_VIDEO_CodingVP8,
                callbacks, appData, component);
 } else if (!strcmp(name, "OMX.google.vp9.decoder")) {
 return new android::SoftVPX(
                name, "video_decoder.vp9", OMX_VIDEO_CodingVP9,
                callbacks, appData, component);
 } else {
        CHECK(!"Unknown component");
 }
 return NULL;
}

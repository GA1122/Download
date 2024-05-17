static AUDIO_OBJECT_TYPE getAOTFromProfile(OMX_U32 profile) {
 if (profile == OMX_AUDIO_AACObjectLC) {
 return AOT_AAC_LC;
 } else if (profile == OMX_AUDIO_AACObjectHE) {
 return AOT_SBR;
 } else if (profile == OMX_AUDIO_AACObjectHE_PS) {
 return AOT_PS;
 } else if (profile == OMX_AUDIO_AACObjectLD) {
 return AOT_ER_AAC_LD;
 } else if (profile == OMX_AUDIO_AACObjectELD) {
 return AOT_ER_AAC_ELD;
 } else {
        ALOGW("Unsupported AAC profile - defaulting to AAC-LC");
 return AOT_AAC_LC;
 }
}

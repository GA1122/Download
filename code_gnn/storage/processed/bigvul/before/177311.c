status_t ACodec::setupAC3Codec(
 bool encoder, int32_t numChannels, int32_t sampleRate) {
 status_t err = setupRawAudioFormat(
            encoder ? kPortIndexInput : kPortIndexOutput, sampleRate, numChannels);

 if (err != OK) {
 return err;
 }

 if (encoder) {
        ALOGW("AC3 encoding is not supported.");
 return INVALID_OPERATION;
 }

    OMX_AUDIO_PARAM_ANDROID_AC3TYPE def;
 InitOMXParams(&def);
    def.nPortIndex = kPortIndexInput;

    err = mOMX->getParameter(
            mNode,
 (OMX_INDEXTYPE)OMX_IndexParamAudioAndroidAc3,
 &def,
 sizeof(def));

 if (err != OK) {
 return err;
 }

    def.nChannels = numChannels;
    def.nSampleRate = sampleRate;

 return mOMX->setParameter(
            mNode,
 (OMX_INDEXTYPE)OMX_IndexParamAudioAndroidAc3,
 &def,
 sizeof(def));
}

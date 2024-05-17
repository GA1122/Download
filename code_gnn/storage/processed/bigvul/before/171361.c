status_t OMXCodec::setAC3Format(int32_t numChannels, int32_t sampleRate) {
    OMX_AUDIO_PARAM_ANDROID_AC3TYPE def;
 InitOMXParams(&def);
    def.nPortIndex = kPortIndexInput;

 status_t err = mOMX->getParameter(
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

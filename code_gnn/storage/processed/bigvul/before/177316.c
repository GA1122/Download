status_t ACodec::setupFlacCodec(
 bool encoder, int32_t numChannels, int32_t sampleRate, int32_t compressionLevel) {

 if (encoder) {
        OMX_AUDIO_PARAM_FLACTYPE def;
 InitOMXParams(&def);
        def.nPortIndex = kPortIndexOutput;

 status_t err = mOMX->getParameter(mNode, OMX_IndexParamAudioFlac, &def, sizeof(def));
 if (err != OK) {
            ALOGE("setupFlacCodec(): Error %d getting OMX_IndexParamAudioFlac parameter", err);
 return err;
 }
        def.nCompressionLevel = compressionLevel;
        err = mOMX->setParameter(mNode, OMX_IndexParamAudioFlac, &def, sizeof(def));
 if (err != OK) {
            ALOGE("setupFlacCodec(): Error %d setting OMX_IndexParamAudioFlac parameter", err);
 return err;
 }
 }

 return setupRawAudioFormat(
            encoder ? kPortIndexInput : kPortIndexOutput,
            sampleRate,
            numChannels);
}

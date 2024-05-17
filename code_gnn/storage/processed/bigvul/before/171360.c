status_t OMXCodec::setAACFormat(
 int32_t numChannels, int32_t sampleRate, int32_t bitRate, int32_t aacProfile, bool isADTS) {
 if (numChannels > 2) {
        ALOGW("Number of channels: (%d) \n", numChannels);
 }

 if (mIsEncoder) {
 if (isADTS) {
 return -EINVAL;
 }

        setRawAudioFormat(kPortIndexInput, sampleRate, numChannels);

        OMX_AUDIO_PARAM_PORTFORMATTYPE format;
 InitOMXParams(&format);
        format.nPortIndex = kPortIndexOutput;
        format.nIndex = 0;
 status_t err = OMX_ErrorNone;
 while (OMX_ErrorNone == err) {
            CHECK_EQ(mOMX->getParameter(mNode, OMX_IndexParamAudioPortFormat,
 &format, sizeof(format)), (status_t)OK);
 if (format.eEncoding == OMX_AUDIO_CodingAAC) {
 break;
 }
            format.nIndex++;
 }
        CHECK_EQ((status_t)OK, err);
        CHECK_EQ(mOMX->setParameter(mNode, OMX_IndexParamAudioPortFormat,
 &format, sizeof(format)), (status_t)OK);

        OMX_PARAM_PORTDEFINITIONTYPE def;
 InitOMXParams(&def);
        def.nPortIndex = kPortIndexOutput;
        CHECK_EQ(mOMX->getParameter(mNode, OMX_IndexParamPortDefinition,
 &def, sizeof(def)), (status_t)OK);
        def.format.audio.bFlagErrorConcealment = OMX_TRUE;
        def.format.audio.eEncoding = OMX_AUDIO_CodingAAC;
        CHECK_EQ(mOMX->setParameter(mNode, OMX_IndexParamPortDefinition,
 &def, sizeof(def)), (status_t)OK);

        OMX_AUDIO_PARAM_AACPROFILETYPE profile;
 InitOMXParams(&profile);
        profile.nPortIndex = kPortIndexOutput;
        CHECK_EQ(mOMX->getParameter(mNode, OMX_IndexParamAudioAac,
 &profile, sizeof(profile)), (status_t)OK);
        profile.nChannels = numChannels;
        profile.eChannelMode = (numChannels == 1?
                OMX_AUDIO_ChannelModeMono: OMX_AUDIO_ChannelModeStereo);
        profile.nSampleRate = sampleRate;
        profile.nBitRate = bitRate;
        profile.nAudioBandWidth = 0;
        profile.nFrameLength = 0;
        profile.nAACtools = OMX_AUDIO_AACToolAll;
        profile.nAACERtools = OMX_AUDIO_AACERNone;
        profile.eAACProfile = (OMX_AUDIO_AACPROFILETYPE) aacProfile;
        profile.eAACStreamFormat = OMX_AUDIO_AACStreamFormatMP4FF;
        err = mOMX->setParameter(mNode, OMX_IndexParamAudioAac,
 &profile, sizeof(profile));

 if (err != OK) {
            CODEC_LOGE("setParameter('OMX_IndexParamAudioAac') failed "
 "(err = %d)",
                       err);
 return err;
 }
 } else {
        OMX_AUDIO_PARAM_AACPROFILETYPE profile;
 InitOMXParams(&profile);
        profile.nPortIndex = kPortIndexInput;

 status_t err = mOMX->getParameter(
                mNode, OMX_IndexParamAudioAac, &profile, sizeof(profile));
        CHECK_EQ(err, (status_t)OK);

        profile.nChannels = numChannels;
        profile.nSampleRate = sampleRate;

        profile.eAACStreamFormat =
            isADTS
 ? OMX_AUDIO_AACStreamFormatMP4ADTS
 : OMX_AUDIO_AACStreamFormatMP4FF;

        err = mOMX->setParameter(
                mNode, OMX_IndexParamAudioAac, &profile, sizeof(profile));

 if (err != OK) {
            CODEC_LOGE("setParameter('OMX_IndexParamAudioAac') failed "
 "(err = %d)",
                       err);
 return err;
 }
 }

 return OK;
}

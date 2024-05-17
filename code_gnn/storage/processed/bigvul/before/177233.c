status_t ACodec::getPortFormat(OMX_U32 portIndex, sp<AMessage> &notify) {
 const char *niceIndex = portIndex == kPortIndexInput ? "input" : "output";
    OMX_PARAM_PORTDEFINITIONTYPE def;
 InitOMXParams(&def);
    def.nPortIndex = portIndex;

 status_t err = mOMX->getParameter(mNode, OMX_IndexParamPortDefinition, &def, sizeof(def));
 if (err != OK) {
 return err;
 }

 if (def.eDir != (portIndex == kPortIndexOutput ? OMX_DirOutput : OMX_DirInput)) {
        ALOGE("unexpected dir: %s(%d) on %s port", asString(def.eDir), def.eDir, niceIndex);
 return BAD_VALUE;
 }

 switch (def.eDomain) {
 case OMX_PortDomainVideo:
 {
            OMX_VIDEO_PORTDEFINITIONTYPE *videoDef = &def.format.video;
 switch ((int)videoDef->eCompressionFormat) {
 case OMX_VIDEO_CodingUnused:
 {
                    CHECK(mIsEncoder ^ (portIndex == kPortIndexOutput));
                    notify->setString("mime", MEDIA_MIMETYPE_VIDEO_RAW);

                    notify->setInt32("stride", videoDef->nStride);
                    notify->setInt32("slice-height", videoDef->nSliceHeight);
                    notify->setInt32("color-format", videoDef->eColorFormat);

 if (mNativeWindow == NULL) {
 DescribeColorFormatParams describeParams;
 InitOMXParams(&describeParams);
                        describeParams.eColorFormat = videoDef->eColorFormat;
                        describeParams.nFrameWidth = videoDef->nFrameWidth;
                        describeParams.nFrameHeight = videoDef->nFrameHeight;
                        describeParams.nStride = videoDef->nStride;
                        describeParams.nSliceHeight = videoDef->nSliceHeight;
                        describeParams.bUsingNativeBuffers = OMX_FALSE;

 if (describeColorFormat(mOMX, mNode, describeParams)) {
                            notify->setBuffer(
 "image-data",
 ABuffer::CreateAsCopy(
 &describeParams.sMediaImage,
 sizeof(describeParams.sMediaImage)));

 MediaImage *img = &describeParams.sMediaImage;
                            ALOGV("[%s] MediaImage { F(%ux%u) @%u+%u+%u @%u+%u+%u @%u+%u+%u }",
                                    mComponentName.c_str(), img->mWidth, img->mHeight,
                                    img->mPlane[0].mOffset, img->mPlane[0].mColInc, img->mPlane[0].mRowInc,
                                    img->mPlane[1].mOffset, img->mPlane[1].mColInc, img->mPlane[1].mRowInc,
                                    img->mPlane[2].mOffset, img->mPlane[2].mColInc, img->mPlane[2].mRowInc);
 }
 }

 if (portIndex != kPortIndexOutput) {
 break;
 }

                    OMX_CONFIG_RECTTYPE rect;
 InitOMXParams(&rect);
                    rect.nPortIndex = portIndex;

 if (mOMX->getConfig(
                                mNode,
 (portIndex == kPortIndexOutput ?
                                        OMX_IndexConfigCommonOutputCrop :
                                        OMX_IndexConfigCommonInputCrop),
 &rect, sizeof(rect)) != OK) {
                        rect.nLeft = 0;
                        rect.nTop = 0;
                        rect.nWidth = videoDef->nFrameWidth;
                        rect.nHeight = videoDef->nFrameHeight;
 }

 if (rect.nLeft < 0 ||
                        rect.nTop < 0 ||
                        rect.nLeft + rect.nWidth > videoDef->nFrameWidth ||
                        rect.nTop + rect.nHeight > videoDef->nFrameHeight) {
                        ALOGE("Wrong cropped rect (%d, %d) - (%u, %u) vs. frame (%u, %u)",
                                rect.nLeft, rect.nTop,
                                rect.nLeft + rect.nWidth, rect.nTop + rect.nHeight,
                                videoDef->nFrameWidth, videoDef->nFrameHeight);
 return BAD_VALUE;
 }

                    notify->setRect(
 "crop",
                            rect.nLeft,
                            rect.nTop,
                            rect.nLeft + rect.nWidth - 1,
                            rect.nTop + rect.nHeight - 1);

 break;
 }

 case OMX_VIDEO_CodingVP8:
 case OMX_VIDEO_CodingVP9:
 {
                    OMX_VIDEO_PARAM_ANDROID_VP8ENCODERTYPE vp8type;
 InitOMXParams(&vp8type);
                    vp8type.nPortIndex = kPortIndexOutput;
 status_t err = mOMX->getParameter(
                            mNode,
 (OMX_INDEXTYPE)OMX_IndexParamVideoAndroidVp8Encoder,
 &vp8type,
 sizeof(vp8type));

 if (err == OK) {
 AString tsSchema = "none";
 if (vp8type.eTemporalPattern
 == OMX_VIDEO_VPXTemporalLayerPatternWebRTC) {
 switch (vp8type.nTemporalLayerCount) {
 case 1:
 {
                                    tsSchema = "webrtc.vp8.1-layer";
 break;
 }
 case 2:
 {
                                    tsSchema = "webrtc.vp8.2-layer";
 break;
 }
 case 3:
 {
                                    tsSchema = "webrtc.vp8.3-layer";
 break;
 }
 default:
 {
 break;
 }
 }
 }
                        notify->setString("ts-schema", tsSchema);
 }
 }

 default:
 {
 if (mIsEncoder ^ (portIndex == kPortIndexOutput)) {
                        ALOGE("Raw port video compression format is %s(%d)",
                                asString(videoDef->eCompressionFormat),
                                videoDef->eCompressionFormat);
 return BAD_VALUE;
 }
 AString mime;
 if (GetMimeTypeForVideoCoding(
                        videoDef->eCompressionFormat, &mime) != OK) {
                        notify->setString("mime", "application/octet-stream");
 } else {
                        notify->setString("mime", mime.c_str());
 }
 break;
 }
 }
            notify->setInt32("width", videoDef->nFrameWidth);
            notify->setInt32("height", videoDef->nFrameHeight);
            ALOGV("[%s] %s format is %s", mComponentName.c_str(),
                    portIndex == kPortIndexInput ? "input" : "output",
                    notify->debugString().c_str());

 break;
 }

 case OMX_PortDomainAudio:
 {
            OMX_AUDIO_PORTDEFINITIONTYPE *audioDef = &def.format.audio;

 switch ((int)audioDef->eEncoding) {
 case OMX_AUDIO_CodingPCM:
 {
                    OMX_AUDIO_PARAM_PCMMODETYPE params;
 InitOMXParams(&params);
                    params.nPortIndex = portIndex;

                    err = mOMX->getParameter(
                            mNode, OMX_IndexParamAudioPcm, &params, sizeof(params));
 if (err != OK) {
 return err;
 }

 if (params.nChannels <= 0
 || (params.nChannels != 1 && !params.bInterleaved)
 || params.nBitPerSample != 16u
 || params.eNumData != OMX_NumericalDataSigned
 || params.ePCMMode != OMX_AUDIO_PCMModeLinear) {
                        ALOGE("unsupported PCM port: %u channels%s, %u-bit, %s(%d), %s(%d) mode ",
                                params.nChannels,
                                params.bInterleaved ? " interleaved" : "",
                                params.nBitPerSample,
                                asString(params.eNumData), params.eNumData,
                                asString(params.ePCMMode), params.ePCMMode);
 return FAILED_TRANSACTION;
 }

                    notify->setString("mime", MEDIA_MIMETYPE_AUDIO_RAW);
                    notify->setInt32("channel-count", params.nChannels);
                    notify->setInt32("sample-rate", params.nSamplingRate);

 if (mChannelMaskPresent) {
                        notify->setInt32("channel-mask", mChannelMask);
 }
 break;
 }

 case OMX_AUDIO_CodingAAC:
 {
                    OMX_AUDIO_PARAM_AACPROFILETYPE params;
 InitOMXParams(&params);
                    params.nPortIndex = portIndex;

                    err = mOMX->getParameter(
                            mNode, OMX_IndexParamAudioAac, &params, sizeof(params));
 if (err != OK) {
 return err;
 }

                    notify->setString("mime", MEDIA_MIMETYPE_AUDIO_AAC);
                    notify->setInt32("channel-count", params.nChannels);
                    notify->setInt32("sample-rate", params.nSampleRate);
 break;
 }

 case OMX_AUDIO_CodingAMR:
 {
                    OMX_AUDIO_PARAM_AMRTYPE params;
 InitOMXParams(&params);
                    params.nPortIndex = portIndex;

                    err = mOMX->getParameter(
                            mNode, OMX_IndexParamAudioAmr, &params, sizeof(params));
 if (err != OK) {
 return err;
 }

                    notify->setInt32("channel-count", 1);
 if (params.eAMRBandMode >= OMX_AUDIO_AMRBandModeWB0) {
                        notify->setString("mime", MEDIA_MIMETYPE_AUDIO_AMR_WB);
                        notify->setInt32("sample-rate", 16000);
 } else {
                        notify->setString("mime", MEDIA_MIMETYPE_AUDIO_AMR_NB);
                        notify->setInt32("sample-rate", 8000);
 }
 break;
 }

 case OMX_AUDIO_CodingFLAC:
 {
                    OMX_AUDIO_PARAM_FLACTYPE params;
 InitOMXParams(&params);
                    params.nPortIndex = portIndex;

                    err = mOMX->getParameter(
                            mNode, OMX_IndexParamAudioFlac, &params, sizeof(params));
 if (err != OK) {
 return err;
 }

                    notify->setString("mime", MEDIA_MIMETYPE_AUDIO_FLAC);
                    notify->setInt32("channel-count", params.nChannels);
                    notify->setInt32("sample-rate", params.nSampleRate);
 break;
 }

 case OMX_AUDIO_CodingMP3:
 {
                    OMX_AUDIO_PARAM_MP3TYPE params;
 InitOMXParams(&params);
                    params.nPortIndex = portIndex;

                    err = mOMX->getParameter(
                            mNode, OMX_IndexParamAudioMp3, &params, sizeof(params));
 if (err != OK) {
 return err;
 }

                    notify->setString("mime", MEDIA_MIMETYPE_AUDIO_MPEG);
                    notify->setInt32("channel-count", params.nChannels);
                    notify->setInt32("sample-rate", params.nSampleRate);
 break;
 }

 case OMX_AUDIO_CodingVORBIS:
 {
                    OMX_AUDIO_PARAM_VORBISTYPE params;
 InitOMXParams(&params);
                    params.nPortIndex = portIndex;

                    err = mOMX->getParameter(
                            mNode, OMX_IndexParamAudioVorbis, &params, sizeof(params));
 if (err != OK) {
 return err;
 }

                    notify->setString("mime", MEDIA_MIMETYPE_AUDIO_VORBIS);
                    notify->setInt32("channel-count", params.nChannels);
                    notify->setInt32("sample-rate", params.nSampleRate);
 break;
 }

 case OMX_AUDIO_CodingAndroidAC3:
 {
                    OMX_AUDIO_PARAM_ANDROID_AC3TYPE params;
 InitOMXParams(&params);
                    params.nPortIndex = portIndex;

                    err = mOMX->getParameter(
                            mNode, (OMX_INDEXTYPE)OMX_IndexParamAudioAndroidAc3,
 &params, sizeof(params));
 if (err != OK) {
 return err;
 }

                    notify->setString("mime", MEDIA_MIMETYPE_AUDIO_AC3);
                    notify->setInt32("channel-count", params.nChannels);
                    notify->setInt32("sample-rate", params.nSampleRate);
 break;
 }

 case OMX_AUDIO_CodingAndroidEAC3:
 {
                    OMX_AUDIO_PARAM_ANDROID_EAC3TYPE params;
 InitOMXParams(&params);
                    params.nPortIndex = portIndex;

                    err = mOMX->getParameter(
                            mNode, (OMX_INDEXTYPE)OMX_IndexParamAudioAndroidEac3,
 &params, sizeof(params));
 if (err != OK) {
 return err;
 }

                    notify->setString("mime", MEDIA_MIMETYPE_AUDIO_EAC3);
                    notify->setInt32("channel-count", params.nChannels);
                    notify->setInt32("sample-rate", params.nSampleRate);
 break;
 }

 case OMX_AUDIO_CodingAndroidOPUS:
 {
                    OMX_AUDIO_PARAM_ANDROID_OPUSTYPE params;
 InitOMXParams(&params);
                    params.nPortIndex = portIndex;

                    err = mOMX->getParameter(
                            mNode, (OMX_INDEXTYPE)OMX_IndexParamAudioAndroidOpus,
 &params, sizeof(params));
 if (err != OK) {
 return err;
 }

                    notify->setString("mime", MEDIA_MIMETYPE_AUDIO_OPUS);
                    notify->setInt32("channel-count", params.nChannels);
                    notify->setInt32("sample-rate", params.nSampleRate);
 break;
 }

 case OMX_AUDIO_CodingG711:
 {
                    OMX_AUDIO_PARAM_PCMMODETYPE params;
 InitOMXParams(&params);
                    params.nPortIndex = portIndex;

                    err = mOMX->getParameter(
                            mNode, (OMX_INDEXTYPE)OMX_IndexParamAudioPcm, &params, sizeof(params));
 if (err != OK) {
 return err;
 }

 const char *mime = NULL;
 if (params.ePCMMode == OMX_AUDIO_PCMModeMULaw) {
                        mime = MEDIA_MIMETYPE_AUDIO_G711_MLAW;
 } else if (params.ePCMMode == OMX_AUDIO_PCMModeALaw) {
                        mime = MEDIA_MIMETYPE_AUDIO_G711_ALAW;
 } else {  
                        mime = MEDIA_MIMETYPE_AUDIO_RAW;
 }
                    notify->setString("mime", mime);
                    notify->setInt32("channel-count", params.nChannels);
                    notify->setInt32("sample-rate", params.nSamplingRate);
 break;
 }

 case OMX_AUDIO_CodingGSMFR:
 {
                    OMX_AUDIO_PARAM_PCMMODETYPE params;
 InitOMXParams(&params);
                    params.nPortIndex = portIndex;

                    err = mOMX->getParameter(
                                mNode, OMX_IndexParamAudioPcm, &params, sizeof(params));
 if (err != OK) {
 return err;
 }

                    notify->setString("mime", MEDIA_MIMETYPE_AUDIO_MSGSM);
                    notify->setInt32("channel-count", params.nChannels);
                    notify->setInt32("sample-rate", params.nSamplingRate);
 break;
 }

 default:
                    ALOGE("Unsupported audio coding: %s(%d)\n",
                            asString(audioDef->eEncoding), audioDef->eEncoding);
 return BAD_TYPE;
 }
 break;
 }

 default:
            ALOGE("Unsupported domain: %s(%d)", asString(def.eDomain), def.eDomain);
 return BAD_TYPE;
 }

 return OK;
}

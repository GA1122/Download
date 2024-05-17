void OMXCodec::dumpPortStatus(OMX_U32 portIndex) {
    OMX_PARAM_PORTDEFINITIONTYPE def;
 InitOMXParams(&def);
    def.nPortIndex = portIndex;

 status_t err = mOMX->getParameter(
            mNode, OMX_IndexParamPortDefinition, &def, sizeof(def));
    CHECK_EQ(err, (status_t)OK);

    printf("%s Port = {\n", portIndex == kPortIndexInput ? "Input" : "Output");

    CHECK((portIndex == kPortIndexInput && def.eDir == OMX_DirInput)
 || (portIndex == kPortIndexOutput && def.eDir == OMX_DirOutput));

    printf("  nBufferCountActual = %" PRIu32 "\n", def.nBufferCountActual);
    printf("  nBufferCountMin = %" PRIu32 "\n", def.nBufferCountMin);
    printf("  nBufferSize = %" PRIu32 "\n", def.nBufferSize);

 switch (def.eDomain) {
 case OMX_PortDomainImage:
 {
 const OMX_IMAGE_PORTDEFINITIONTYPE *imageDef = &def.format.image;

            printf("\n");
            printf("  // Image\n");
            printf("  nFrameWidth = %" PRIu32 "\n", imageDef->nFrameWidth);
            printf("  nFrameHeight = %" PRIu32 "\n", imageDef->nFrameHeight);
            printf("  nStride = %" PRIu32 "\n", imageDef->nStride);

            printf("  eCompressionFormat = %s\n",
                   asString(imageDef->eCompressionFormat));

            printf("  eColorFormat = %s\n",
                   asString(imageDef->eColorFormat));

 break;
 }

 case OMX_PortDomainVideo:
 {
            OMX_VIDEO_PORTDEFINITIONTYPE *videoDef = &def.format.video;

            printf("\n");
            printf("  // Video\n");
            printf("  nFrameWidth = %" PRIu32 "\n", videoDef->nFrameWidth);
            printf("  nFrameHeight = %" PRIu32 "\n", videoDef->nFrameHeight);
            printf("  nStride = %" PRIu32 "\n", videoDef->nStride);

            printf("  eCompressionFormat = %s\n",
                   asString(videoDef->eCompressionFormat));

            printf("  eColorFormat = %s\n",
                   asString(videoDef->eColorFormat));

 break;
 }

 case OMX_PortDomainAudio:
 {
            OMX_AUDIO_PORTDEFINITIONTYPE *audioDef = &def.format.audio;

            printf("\n");
            printf("  // Audio\n");
            printf("  eEncoding = %s\n",
                   asString(audioDef->eEncoding));

 if (audioDef->eEncoding == OMX_AUDIO_CodingPCM) {
                OMX_AUDIO_PARAM_PCMMODETYPE params;
 InitOMXParams(&params);
                params.nPortIndex = portIndex;

                err = mOMX->getParameter(
                        mNode, OMX_IndexParamAudioPcm, &params, sizeof(params));
                CHECK_EQ(err, (status_t)OK);

                printf("  nSamplingRate = %" PRIu32 "\n", params.nSamplingRate);
                printf("  nChannels = %" PRIu32 "\n", params.nChannels);
                printf("  bInterleaved = %d\n", params.bInterleaved);
                printf("  nBitPerSample = %" PRIu32 "\n", params.nBitPerSample);

                printf("  eNumData = %s\n",
                       params.eNumData == OMX_NumericalDataSigned
 ? "signed" : "unsigned");

                printf("  ePCMMode = %s\n", asString(params.ePCMMode));
 } else if (audioDef->eEncoding == OMX_AUDIO_CodingAMR) {
                OMX_AUDIO_PARAM_AMRTYPE amr;
 InitOMXParams(&amr);
                amr.nPortIndex = portIndex;

                err = mOMX->getParameter(
                        mNode, OMX_IndexParamAudioAmr, &amr, sizeof(amr));
                CHECK_EQ(err, (status_t)OK);

                printf("  nChannels = %" PRIu32 "\n", amr.nChannels);
                printf("  eAMRBandMode = %s\n",
                        asString(amr.eAMRBandMode));
                printf("  eAMRFrameFormat = %s\n",
                        asString(amr.eAMRFrameFormat));
 }

 break;
 }

 default:
 {
            printf("  // Unknown\n");
 break;
 }
 }

    printf("}\n");
}

status_t ACodec::selectAudioPortFormat(
        OMX_U32 portIndex, OMX_AUDIO_CODINGTYPE desiredFormat) {
    OMX_AUDIO_PARAM_PORTFORMATTYPE format;
 InitOMXParams(&format);

    format.nPortIndex = portIndex;
 for (OMX_U32 index = 0;; ++index) {
        format.nIndex = index;

 status_t err = mOMX->getParameter(
                mNode, OMX_IndexParamAudioPortFormat,
 &format, sizeof(format));

 if (err != OK) {
 return err;
 }

 if (format.eEncoding == desiredFormat) {
 break;
 }
 }

 return mOMX->setParameter(
            mNode, OMX_IndexParamAudioPortFormat, &format, sizeof(format));
}

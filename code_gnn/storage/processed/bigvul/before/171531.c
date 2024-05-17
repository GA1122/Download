bool SimpleSoftOMXComponent::isSetParameterAllowed(
        OMX_INDEXTYPE index, const OMX_PTR params) const {
 if (mState == OMX_StateLoaded) {
 return true;
 }

    OMX_U32 portIndex;

 switch (index) {
 case OMX_IndexParamPortDefinition:
 {
            portIndex = ((OMX_PARAM_PORTDEFINITIONTYPE *)params)->nPortIndex;
 break;
 }

 case OMX_IndexParamAudioPcm:
 {
            portIndex = ((OMX_AUDIO_PARAM_PCMMODETYPE *)params)->nPortIndex;
 break;
 }

 case OMX_IndexParamAudioAac:
 {
            portIndex = ((OMX_AUDIO_PARAM_AACPROFILETYPE *)params)->nPortIndex;
 break;
 }

 default:
 return false;
 }

    CHECK(portIndex < mPorts.size());

 return !mPorts.itemAt(portIndex).mDef.bEnabled;
}

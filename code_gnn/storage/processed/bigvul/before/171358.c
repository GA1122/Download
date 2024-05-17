static OMX_AUDIO_AMRBANDMODETYPE pickModeFromBitRate(bool isAMRWB, int32_t bps) {
 if (isAMRWB) {
 if (bps <= 6600) {
 return OMX_AUDIO_AMRBandModeWB0;
 } else if (bps <= 8850) {
 return OMX_AUDIO_AMRBandModeWB1;
 } else if (bps <= 12650) {
 return OMX_AUDIO_AMRBandModeWB2;
 } else if (bps <= 14250) {
 return OMX_AUDIO_AMRBandModeWB3;
 } else if (bps <= 15850) {
 return OMX_AUDIO_AMRBandModeWB4;
 } else if (bps <= 18250) {
 return OMX_AUDIO_AMRBandModeWB5;
 } else if (bps <= 19850) {
 return OMX_AUDIO_AMRBandModeWB6;
 } else if (bps <= 23050) {
 return OMX_AUDIO_AMRBandModeWB7;
 }

 return OMX_AUDIO_AMRBandModeWB8;
 } else {  
 if (bps <= 4750) {
 return OMX_AUDIO_AMRBandModeNB0;
 } else if (bps <= 5150) {
 return OMX_AUDIO_AMRBandModeNB1;
 } else if (bps <= 5900) {
 return OMX_AUDIO_AMRBandModeNB2;
 } else if (bps <= 6700) {
 return OMX_AUDIO_AMRBandModeNB3;
 } else if (bps <= 7400) {
 return OMX_AUDIO_AMRBandModeNB4;
 } else if (bps <= 7950) {
 return OMX_AUDIO_AMRBandModeNB5;
 } else if (bps <= 10200) {
 return OMX_AUDIO_AMRBandModeNB6;
 }

 return OMX_AUDIO_AMRBandModeNB7;
 }
}

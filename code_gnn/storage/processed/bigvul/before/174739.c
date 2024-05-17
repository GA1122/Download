void WT_ProcessVoice (S_WT_VOICE *pWTVoice, S_WT_INT_FRAME *pWTIntFrame)
{

  
 if (pWTVoice->loopStart== WT_NOISE_GENERATOR)
 {
        WT_NoiseGenerator(pWTVoice, pWTIntFrame);
        WT_VoiceGain(pWTVoice, pWTIntFrame);
 }

  
 else
 {
        WT_InterpolateMono(pWTVoice, pWTIntFrame);
 }
}

 void WT_InterpolateNoLoop (S_WT_VOICE *pWTVoice, S_WT_INT_FRAME *pWTIntFrame)
 {
     EAS_PCM *pOutputBuffer;
     EAS_I32 phaseInc;
     EAS_I32 phaseFrac;
     EAS_I32 acc0;
  const EAS_SAMPLE *pSamples;
     EAS_I32 samp1;
     EAS_I32 samp2;
     EAS_I32 numSamples;
 
  
       
      numSamples = pWTIntFrame->numSamples;
//     if (numSamples <= 0) {
//         ALOGE("b/26366256");
//         return;
//     }
      pOutputBuffer = pWTIntFrame->pAudioBuffer;
  
      phaseInc = pWTIntFrame->frame.phaseIncrement;
     pSamples = (const EAS_SAMPLE*) pWTVoice->phaseAccum;
     phaseFrac = (EAS_I32)pWTVoice->phaseFrac;
 
   
 #if defined(_8_BIT_SAMPLES)
   
     samp1 = pSamples[0] << 8;
   
     samp2 = pSamples[1] << 8;
 #else
     samp1 = pSamples[0];
     samp2 = pSamples[1];
 #endif
 
  while (numSamples--) {
 
 
   
         acc0 = samp2 - samp1;
         acc0 = acc0 * phaseFrac;
   
         acc0 = samp1 + (acc0 >> NUM_PHASE_FRAC_BITS);
 
   
   
  *pOutputBuffer++ = (EAS_I16)(acc0 >> 2);
 
   
         phaseFrac += phaseInc;
   
         acc0 = phaseFrac >> NUM_PHASE_FRAC_BITS;
 
   
  if (acc0 > 0) {
 
   
             pSamples += acc0;
             phaseFrac = (EAS_I32)((EAS_U32)phaseFrac & PHASE_FRAC_MASK);
 
   
 #if defined(_8_BIT_SAMPLES)
   
             samp1 = pSamples[0] << 8;
   
             samp2 = pSamples[1] << 8;
 #else
             samp1 = pSamples[0];
             samp2 = pSamples[1];
 #endif
  }
  }
 
   
     pWTVoice->phaseAccum = (EAS_U32) pSamples;
     pWTVoice->phaseFrac = (EAS_U32) phaseFrac;
 }
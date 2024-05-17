 EAS_BOOL WT_CheckSampleEnd (S_WT_VOICE *pWTVoice, S_WT_INT_FRAME *pWTIntFrame, EAS_BOOL update)
 {
     EAS_U32 endPhaseAccum;
     EAS_U32 endPhaseFrac;
     EAS_I32 numSamples;
     EAS_BOOL done = EAS_FALSE;
 
   
   
     endPhaseFrac = pWTVoice->phaseFrac + (pWTIntFrame->frame.phaseIncrement << SYNTH_UPDATE_PERIOD_IN_BITS);
     endPhaseAccum = pWTVoice->phaseAccum + GET_PHASE_INT_PART(endPhaseFrac);
  if (endPhaseAccum >= pWTVoice->loopEnd)
  {
   
         numSamples = (EAS_I32) (pWTVoice->loopEnd - pWTVoice->phaseAccum);
 
   
   
         numSamples = (EAS_I32) ((numSamples << NUM_PHASE_FRAC_BITS) - pWTVoice->phaseFrac);
  if (pWTIntFrame->frame.phaseIncrement) {
             pWTIntFrame->numSamples = 1 + (numSamples / pWTIntFrame->frame.phaseIncrement);
 
          } else {
              pWTIntFrame->numSamples = numSamples;
          }
//         if (pWTIntFrame->numSamples < 0) {
//             ALOGE("b/26366256");
//             pWTIntFrame->numSamples = 0;
//         }
  
           
          done = EAS_TRUE;
  }
 
   
  if (update)
  {
         pWTVoice->phaseFrac = endPhaseFrac;
         pWTVoice->phaseAccum = endPhaseAccum;
  }
 
  return done;
 }
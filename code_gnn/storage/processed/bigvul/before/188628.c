 void WT_VoiceGain (S_WT_VOICE *pWTVoice, S_WT_INT_FRAME *pWTIntFrame)
 {
     EAS_I32 *pMixBuffer;
     EAS_PCM *pInputBuffer;
     EAS_I32 gain;
     EAS_I32 gainIncrement;
     EAS_I32 tmp0;
     EAS_I32 tmp1;
     EAS_I32 tmp2;
     EAS_I32 numSamples;
 
 #if (NUM_OUTPUT_CHANNELS == 2)
     EAS_I32 gainLeft, gainRight;
 #endif
 
   
 
      numSamples = pWTIntFrame->numSamples;
      if (numSamples <= 0) {
          ALOGE("b/26366256");
//         android_errorWriteLog(0x534e4554, "26366256");
          return;
      }
      pMixBuffer = pWTIntFrame->pMixBuffer;
     pInputBuffer = pWTIntFrame->pAudioBuffer;
 
   
     gainIncrement = (pWTIntFrame->frame.gainTarget - pWTIntFrame->prevGain) << (16 - SYNTH_UPDATE_PERIOD_IN_BITS);
  if (gainIncrement < 0)
         gainIncrement++;
   
     gain = pWTIntFrame->prevGain << 16;
 
 #if (NUM_OUTPUT_CHANNELS == 2)
     gainLeft = pWTVoice->gainLeft;
     gainRight = pWTVoice->gainRight;
 #endif
 
  while (numSamples--) {
 
   
         tmp0 = *pInputBuffer++;
         gain += gainIncrement;
   
         tmp2 = gain >> 16;
 
   
         tmp2 *= tmp0;
 
 
   
 #if (NUM_OUTPUT_CHANNELS == 2)
   
         tmp2 = tmp2 >> 14;
 
   
         tmp1 = *pMixBuffer;
 
   
         tmp0 = tmp2 * gainLeft;
   
         tmp0 = tmp0 >> NUM_MIXER_GUARD_BITS;
         tmp1 += tmp0;
  *pMixBuffer++ = tmp1;
 
   
         tmp1 = *pMixBuffer;
 
   
         tmp0 = tmp2 * gainRight;
   
         tmp0 = tmp0 >> NUM_MIXER_GUARD_BITS;
         tmp1 += tmp0;
  *pMixBuffer++ = tmp1;
 
   
 #else
 
   
         tmp1 = *pMixBuffer;
   
         tmp2 = tmp2 >> (NUM_MIXER_GUARD_BITS - 1);
         tmp1 += tmp2;
  *pMixBuffer++ = tmp1;
 #endif
 
  }
 }
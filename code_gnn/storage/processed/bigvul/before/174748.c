static void WT_UpdateEG2 (S_WT_VOICE *pWTVoice, const S_ENVELOPE *pEnv)
{
    EAS_I32 temp;

 switch (pWTVoice->eg2State)
 {
 case eEnvelopeStateAttack:
            temp = pWTVoice->eg2Value + pWTVoice->eg2Increment;

  
 if (temp >= SYNTH_FULL_SCALE_EG1_GAIN)
 {
  
                temp = SYNTH_FULL_SCALE_EG1_GAIN;

  
                pWTVoice->eg2State = eEnvelopeStateDecay;

                pWTVoice->eg2Increment = pEnv->decayTime;
 }

 break;

  
 case eEnvelopeStateDecay:
            temp = pWTVoice->eg2Value -pWTVoice->eg2Increment;

  
 if (temp <= pEnv->sustainLevel)
 {
  
                temp = pEnv->sustainLevel;

  
                pWTVoice->eg2State = eEnvelopeStateSustain;
 }
 break;

 case eEnvelopeStateSustain:
 return;

 case eEnvelopeStateRelease:
            temp = pWTVoice->eg2Value - pWTVoice->eg2Increment;

 if (temp <= 0)
 {
                temp = 0;
                pWTVoice->eg2State = eEnvelopeStateMuted;
 }

 break;

  
 case eEnvelopeStateMuted:
            temp = 0;
 break;

 case eEnvelopeStateInvalid:
 default:
            temp = 0;
#ifdef  _DEBUG_SYNTH
 {   }
#endif
 break;
 }

    pWTVoice->eg2Value = (EAS_I16) temp;
}

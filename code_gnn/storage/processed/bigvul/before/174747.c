static void WT_UpdateEG1 (S_WT_VOICE *pWTVoice, const S_ENVELOPE *pEnv)
{
    EAS_I32 temp;

 switch (pWTVoice->eg1State)
 {
 case eEnvelopeStateAttack:
            temp = pWTVoice->eg1Value + pWTVoice->eg1Increment;

  
 if (temp >= SYNTH_FULL_SCALE_EG1_GAIN)
 {
  
                temp = SYNTH_FULL_SCALE_EG1_GAIN;

  
                pWTVoice->eg1State = eEnvelopeStateDecay;
                pWTVoice->eg1Increment = pEnv->decayTime;
 }

 break;

  
 case eEnvelopeStateDecay:
            temp = MULT_EG1_EG1(pWTVoice->eg1Value, pWTVoice->eg1Increment);

  
 if (temp <= pEnv->sustainLevel)
 {
  
                temp = pEnv->sustainLevel;

  
 if (temp > 0)
                    pWTVoice->eg1State = eEnvelopeStateSustain;

  
 else
                    pWTVoice->eg1State = eEnvelopeStateMuted;
 }

 break;

 case eEnvelopeStateSustain:
 return;

 case eEnvelopeStateRelease:
            temp = MULT_EG1_EG1(pWTVoice->eg1Value, pWTVoice->eg1Increment);

  
 if (temp <= 0)
 {
                temp = 0;
                pWTVoice->eg1State = eEnvelopeStateMuted;
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

    pWTVoice->eg1Value = (EAS_I16) temp;
}

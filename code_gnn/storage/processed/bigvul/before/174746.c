static void WT_UpdateChannel (S_VOICE_MGR *pVoiceMgr, S_SYNTH *pSynth, EAS_U8 channel)
{
    EAS_I32 staticGain;
    EAS_I32 pitchBend;
    S_SYNTH_CHANNEL *pChannel;

    pChannel = &pSynth->channels[channel];

  
    staticGain = MULT_EG1_EG1((pChannel->volume) << (NUM_EG1_FRAC_BITS - 7),
 (pChannel->expression) << (NUM_EG1_FRAC_BITS - 7));

  
    staticGain = MULT_EG1_EG1(staticGain, staticGain);

    pChannel->staticGain = (EAS_I16) MULT_EG1_EG1(staticGain, pSynth->masterVolume);

  
  
    pitchBend =
 (((EAS_I32)(pChannel->pitchBend) << 2)
 - 32768);

    pChannel->staticPitch =
        MULT_EG1_EG1(pitchBend, pChannel->pitchBendSensitivity);

  
 if (!(pChannel->channelFlags & CHANNEL_FLAG_RHYTHM_CHANNEL))
        pChannel->staticPitch += pChannel->finePitch + (pChannel->coarsePitch * 100);

  
    pChannel->channelFlags &= ~CHANNEL_FLAG_UPDATE_CHANNEL_PARAMETERS;
 return;
}

mapInvfMode (INVF_MODE mode,
             INVF_MODE prevMode,
             WHITENING_FACTORS whFactors)
{
 switch (mode) {
 case INVF_LOW_LEVEL:
 if(prevMode == INVF_OFF)
 return whFactors.transitionLevel;
 else
 return whFactors.lowLevel;

 case INVF_MID_LEVEL:
 return whFactors.midLevel;

 case INVF_HIGH_LEVEL:
 return whFactors.highLevel;

 default:
 if(prevMode == INVF_LOW_LEVEL)
 return whFactors.transitionLevel;
 else
 return whFactors.off;
 }
}

int32_t EqualizerGetBand(EffectContext *pContext __unused, uint32_t targetFreq){
 int band = 0;

 if(targetFreq < bandFreqRange[0][0]){
 return -EINVAL;
 }else if(targetFreq == bandFreqRange[0][0]){
 return 0;
 }
 for(int i=0; i<FIVEBAND_NUMBANDS;i++){
 if((targetFreq > bandFreqRange[i][0])&&(targetFreq <= bandFreqRange[i][1])){
            band = i;
 }
 }
 return band;
}

int16_t LVC_Convert_VolToDb(uint32_t vol){
 int16_t  dB;

    dB = LVC_ToDB_s32Tos16(vol <<7);
    dB = (dB +8)>>4;
    dB = (dB <-96) ? -96 : dB ;

 return dB;
}

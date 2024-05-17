static size_t getFrameSize(bool isWide, unsigned FT) {
 static const size_t kFrameSizeNB[16] = {
 95, 103, 118, 134, 148, 159, 204, 244,
 39, 43, 38, 37,  
 0, 0, 0,  
 0  
 };
 static const size_t kFrameSizeWB[16] = {
 132, 177, 253, 285, 317, 365, 397, 461, 477,
 40,  
 0, 0, 0, 0,  
 0,  
 0  
 };

 if (FT > 15 || (isWide && FT > 9 && FT < 14) || (!isWide && FT > 11 && FT < 15)) {
        ALOGE("illegal AMR frame type %d", FT);
 return 0;
 }

 size_t frameSize = isWide ? kFrameSizeWB[FT] : kFrameSizeNB[FT];

    frameSize = (frameSize + 7) / 8 + 1;

 return frameSize;
}

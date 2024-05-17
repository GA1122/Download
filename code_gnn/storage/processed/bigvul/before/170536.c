void Downmix_foldFrom5Point1(int16_t *pSrc, int16_t*pDst, size_t numFrames, bool accumulate) {
 int32_t lt, rt, centerPlusLfeContrib;  
 if (accumulate) {
 while (numFrames) {
            centerPlusLfeContrib = (pSrc[2] * MINUS_3_DB_IN_Q19_12)
 + (pSrc[3] * MINUS_3_DB_IN_Q19_12);
            lt = (pSrc[0] << 12) + centerPlusLfeContrib + (pSrc[4] << 12);
            rt = (pSrc[1] << 12) + centerPlusLfeContrib + (pSrc[5] << 12);
            pDst[0] = clamp16(pDst[0] + (lt >> 13));
            pDst[1] = clamp16(pDst[1] + (rt >> 13));
            pSrc += 6;
            pDst += 2;
            numFrames--;
 }
 } else {  
 while (numFrames) {
            centerPlusLfeContrib = (pSrc[2] * MINUS_3_DB_IN_Q19_12)
 + (pSrc[3] * MINUS_3_DB_IN_Q19_12);
            lt = (pSrc[0] << 12) + centerPlusLfeContrib + (pSrc[4] << 12);
            rt = (pSrc[1] << 12) + centerPlusLfeContrib + (pSrc[5] << 12);
            pDst[0] = clamp16(lt >> 13);  
            pDst[1] = clamp16(rt >> 13);  
            pSrc += 6;
            pDst += 2;
            numFrames--;
 }
 }
}

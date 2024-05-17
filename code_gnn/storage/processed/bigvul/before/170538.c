void Downmix_foldFromQuad(int16_t *pSrc, int16_t*pDst, size_t numFrames, bool accumulate) {
 if (accumulate) {
 while (numFrames) {
            pDst[0] = clamp16(pDst[0] + ((pSrc[0] + pSrc[2]) >> 1));
            pDst[1] = clamp16(pDst[1] + ((pSrc[1] + pSrc[3]) >> 1));
            pSrc += 4;
            pDst += 2;
            numFrames--;
 }
 } else {  
 while (numFrames) {
            pDst[0] = clamp16((pSrc[0] + pSrc[2]) >> 1);
            pDst[1] = clamp16((pSrc[1] + pSrc[3]) >> 1);
            pSrc += 4;
            pDst += 2;
            numFrames--;
 }
 }
}

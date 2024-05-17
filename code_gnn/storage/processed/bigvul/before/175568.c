status_t HevcParameterSets::parseSps(const uint8_t* data, size_t size) {
 NALBitReader reader(data, size);
    reader.skipBits(4);
 uint8_t maxSubLayersMinus1 = reader.getBitsWithFallback(3, 0);
    reader.skipBits(1);
    reader.skipBits(96);
 if (maxSubLayersMinus1 > 0) {
 bool subLayerProfilePresentFlag[8];
 bool subLayerLevelPresentFlag[8];
 for (int i = 0; i < maxSubLayersMinus1; ++i) {
            subLayerProfilePresentFlag[i] = reader.getBitsWithFallback(1, 0);
            subLayerLevelPresentFlag[i] = reader.getBitsWithFallback(1, 0);
 }
        reader.skipBits(2 * (8 - maxSubLayersMinus1));
 for (int i = 0; i < maxSubLayersMinus1; ++i) {
 if (subLayerProfilePresentFlag[i]) {
                reader.skipBits(88);
 }
 if (subLayerLevelPresentFlag[i]) {
                reader.skipBits(8);
 }
 }
 }
    skipUE(&reader);
 uint8_t chromaFormatIdc = parseUEWithFallback(&reader, 0);
    mParams.add(kChromaFormatIdc, chromaFormatIdc);
 if (chromaFormatIdc == 3) {
        reader.skipBits(1);
 }
    skipUE(&reader);
    skipUE(&reader);
 if (reader.getBitsWithFallback(1, 0)  ) {
        skipUE(&reader);
        skipUE(&reader);
        skipUE(&reader);
        skipUE(&reader);
 }
    mParams.add(kBitDepthLumaMinus8, parseUEWithFallback(&reader, 0));
    mParams.add(kBitDepthChromaMinus8, parseUEWithFallback(&reader, 0));

 size_t log2MaxPicOrderCntLsb = parseUEWithFallback(&reader, 0) + (size_t)4;
 bool spsSubLayerOrderingInfoPresentFlag = reader.getBitsWithFallback(1, 0);
 for (uint32_t i = spsSubLayerOrderingInfoPresentFlag ? 0 : maxSubLayersMinus1;
            i <= maxSubLayersMinus1; ++i) {
        skipUE(&reader);  
        skipUE(&reader);  
        skipUE(&reader);  
 }

    skipUE(&reader);  
    skipUE(&reader);  
    skipUE(&reader);  
    skipUE(&reader);  
    skipUE(&reader);  
    skipUE(&reader);  
 if (reader.getBitsWithFallback(1, 0)) {  
 if (reader.getBitsWithFallback(1, 0)) {  
 for (uint32_t sizeId = 0; sizeId < 4; ++sizeId) {
 for (uint32_t matrixId = 0; matrixId < 6; matrixId += (sizeId == 3) ? 3 : 1) {
 if (!reader.getBitsWithFallback(1, 1)) {
                        skipUE(&reader);  
 } else {
 uint32_t coefNum = std::min(64, (1 << (4 + (sizeId << 1))));
 if (sizeId > 1) {
                            skipSE(&reader);  
 }
 for (uint32_t i = 0; i < coefNum; ++i) {
                            skipSE(&reader);  
 }
 }
 }
 }
 }
 }
    reader.skipBits(1);  
    reader.skipBits(1);  
 if (reader.getBitsWithFallback(1, 0)) {  
        reader.skipBits(4);  
        reader.skipBits(4);  
        skipUE(&reader);  
        skipUE(&reader);  
        reader.skipBits(1);  
 }
 uint32_t numShortTermRefPicSets = parseUEWithFallback(&reader, 0);
 uint32_t numPics = 0;
 for (uint32_t i = 0; i < numShortTermRefPicSets; ++i) {
 if (i != 0 && reader.getBitsWithFallback(1, 0)) {  
            reader.skipBits(1);  
            skipUE(&reader);  
 uint32_t nextNumPics = 0;
 for (uint32_t j = 0; j <= numPics; ++j) {
 if (reader.getBitsWithFallback(1, 0)  
 || reader.getBitsWithFallback(1, 0)) {  
 ++nextNumPics;
 }
 }
            numPics = nextNumPics;
 } else {
 uint32_t numNegativePics = parseUEWithFallback(&reader, 0);
 uint32_t numPositivePics = parseUEWithFallback(&reader, 0);
 if (numNegativePics > UINT32_MAX - numPositivePics) {
 return ERROR_MALFORMED;
 }
            numPics = numNegativePics + numPositivePics;
 for (uint32_t j = 0; j < numPics; ++j) {
                skipUE(&reader);  
                reader.skipBits(1);  
 }
 }
 }
 if (reader.getBitsWithFallback(1, 0)) {  
 uint32_t numLongTermRefPicSps = parseUEWithFallback(&reader, 0);
 for (uint32_t i = 0; i < numLongTermRefPicSps; ++i) {
            reader.skipBits(log2MaxPicOrderCntLsb);  
            reader.skipBits(1);  
 }
 }
    reader.skipBits(1);  
    reader.skipBits(1);  
 if (reader.getBitsWithFallback(1, 0)) {  
 if (reader.getBitsWithFallback(1, 0)) {  
 uint32_t aspectRatioIdc = reader.getBitsWithFallback(8, 0);
 if (aspectRatioIdc == 0xFF  ) {
                reader.skipBits(16);  
                reader.skipBits(16);  
 }
 }
 if (reader.getBitsWithFallback(1, 0)) {  
            reader.skipBits(1);  
 }
 if (reader.getBitsWithFallback(1, 0)) {  
            reader.skipBits(3);  
 uint32_t videoFullRangeFlag;
 if (reader.getBitsGraceful(1, &videoFullRangeFlag)) {
                mParams.add(kVideoFullRangeFlag, videoFullRangeFlag);
 }
 if (reader.getBitsWithFallback(1, 0)) {  
                mInfo = (Info)(mInfo | kInfoHasColorDescription);
 uint32_t colourPrimaries, transferCharacteristics, matrixCoeffs;
 if (reader.getBitsGraceful(8, &colourPrimaries)) {
                    mParams.add(kColourPrimaries, colourPrimaries);
 }
 if (reader.getBitsGraceful(8, &transferCharacteristics)) {
                    mParams.add(kTransferCharacteristics, transferCharacteristics);
 if (transferCharacteristics == 16  
 || transferCharacteristics == 18  ) {
                        mInfo = (Info)(mInfo | kInfoIsHdr);
 }
 }
 if (reader.getBitsGraceful(8, &matrixCoeffs)) {
                    mParams.add(kMatrixCoeffs, matrixCoeffs);
 }
 }
 }
 }

 return reader.overRead() ? ERROR_MALFORMED : OK;
}

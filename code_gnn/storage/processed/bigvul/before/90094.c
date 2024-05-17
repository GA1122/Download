static size_t ZSTD_loadZstdDictionary(ZSTD_compressedBlockState_t* bs,
                                      ZSTD_matchState_t* ms,
                                      ZSTD_CCtx_params const* params,
                                      const void* dict, size_t dictSize,
                                      ZSTD_dictTableLoadMethod_e dtlm,
                                      void* workspace)
{
    const BYTE* dictPtr = (const BYTE*)dict;
    const BYTE* const dictEnd = dictPtr + dictSize;
    short offcodeNCount[MaxOff+1];
    unsigned offcodeMaxValue = MaxOff;
    size_t dictID;

    ZSTD_STATIC_ASSERT(HUF_WORKSPACE_SIZE >= (1<<MAX(MLFSELog,LLFSELog)));
    assert(dictSize > 8);
    assert(MEM_readLE32(dictPtr) == ZSTD_MAGIC_DICTIONARY);

    dictPtr += 4;    
    dictID = params->fParams.noDictIDFlag ? 0 :  MEM_readLE32(dictPtr);
    dictPtr += 4;

    {   unsigned maxSymbolValue = 255;
        size_t const hufHeaderSize = HUF_readCTable((HUF_CElt*)bs->entropy.huf.CTable, &maxSymbolValue, dictPtr, dictEnd-dictPtr);
        if (HUF_isError(hufHeaderSize)) return ERROR(dictionary_corrupted);
        if (maxSymbolValue < 255) return ERROR(dictionary_corrupted);
        dictPtr += hufHeaderSize;
    }

    {   unsigned offcodeLog;
        size_t const offcodeHeaderSize = FSE_readNCount(offcodeNCount, &offcodeMaxValue, &offcodeLog, dictPtr, dictEnd-dictPtr);
        if (FSE_isError(offcodeHeaderSize)) return ERROR(dictionary_corrupted);
        if (offcodeLog > OffFSELog) return ERROR(dictionary_corrupted);
         
         
        CHECK_E( FSE_buildCTable_wksp(bs->entropy.fse.offcodeCTable, offcodeNCount, MaxOff, offcodeLog, workspace, HUF_WORKSPACE_SIZE),
                 dictionary_corrupted);
        dictPtr += offcodeHeaderSize;
    }

    {   short matchlengthNCount[MaxML+1];
        unsigned matchlengthMaxValue = MaxML, matchlengthLog;
        size_t const matchlengthHeaderSize = FSE_readNCount(matchlengthNCount, &matchlengthMaxValue, &matchlengthLog, dictPtr, dictEnd-dictPtr);
        if (FSE_isError(matchlengthHeaderSize)) return ERROR(dictionary_corrupted);
        if (matchlengthLog > MLFSELog) return ERROR(dictionary_corrupted);
         
        CHECK_F( ZSTD_checkDictNCount(matchlengthNCount, matchlengthMaxValue, MaxML));
        CHECK_E( FSE_buildCTable_wksp(bs->entropy.fse.matchlengthCTable, matchlengthNCount, matchlengthMaxValue, matchlengthLog, workspace, HUF_WORKSPACE_SIZE),
                 dictionary_corrupted);
        dictPtr += matchlengthHeaderSize;
    }

    {   short litlengthNCount[MaxLL+1];
        unsigned litlengthMaxValue = MaxLL, litlengthLog;
        size_t const litlengthHeaderSize = FSE_readNCount(litlengthNCount, &litlengthMaxValue, &litlengthLog, dictPtr, dictEnd-dictPtr);
        if (FSE_isError(litlengthHeaderSize)) return ERROR(dictionary_corrupted);
        if (litlengthLog > LLFSELog) return ERROR(dictionary_corrupted);
         
        CHECK_F( ZSTD_checkDictNCount(litlengthNCount, litlengthMaxValue, MaxLL));
        CHECK_E( FSE_buildCTable_wksp(bs->entropy.fse.litlengthCTable, litlengthNCount, litlengthMaxValue, litlengthLog, workspace, HUF_WORKSPACE_SIZE),
                 dictionary_corrupted);
        dictPtr += litlengthHeaderSize;
    }

    if (dictPtr+12 > dictEnd) return ERROR(dictionary_corrupted);
    bs->rep[0] = MEM_readLE32(dictPtr+0);
    bs->rep[1] = MEM_readLE32(dictPtr+4);
    bs->rep[2] = MEM_readLE32(dictPtr+8);
    dictPtr += 12;

    {   size_t const dictContentSize = (size_t)(dictEnd - dictPtr);
        U32 offcodeMax = MaxOff;
        if (dictContentSize <= ((U32)-1) - 128 KB) {
            U32 const maxOffset = (U32)dictContentSize + 128 KB;  
            offcodeMax = ZSTD_highbit32(maxOffset);  
        }
         
        CHECK_F (ZSTD_checkDictNCount(offcodeNCount, offcodeMaxValue, MIN(offcodeMax, MaxOff)));
         
        {   U32 u;
            for (u=0; u<3; u++) {
                if (bs->rep[u] == 0) return ERROR(dictionary_corrupted);
                if (bs->rep[u] > dictContentSize) return ERROR(dictionary_corrupted);
        }   }

        bs->entropy.huf.repeatMode = HUF_repeat_valid;
        bs->entropy.fse.offcode_repeatMode = FSE_repeat_valid;
        bs->entropy.fse.matchlength_repeatMode = FSE_repeat_valid;
        bs->entropy.fse.litlength_repeatMode = FSE_repeat_valid;
        CHECK_F(ZSTD_loadDictionaryContent(ms, params, dictPtr, dictContentSize, dtlm));
        return dictID;
    }
}
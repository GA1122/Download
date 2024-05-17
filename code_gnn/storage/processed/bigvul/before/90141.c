static int basicUnitTests(U32 seed, double compressibility)
{
    size_t const CNBuffSize = 5 MB;
    void* const CNBuffer = malloc(CNBuffSize);
    size_t const compressedBufferSize = ZSTD_compressBound(CNBuffSize);
    void* const compressedBuffer = malloc(compressedBufferSize);
    void* const decodedBuffer = malloc(CNBuffSize);
    ZSTD_DCtx* dctx = ZSTD_createDCtx();
    int testResult = 0;
    U32 testNb=0;
    size_t cSize;

     
    if (!CNBuffer || !compressedBuffer || !decodedBuffer) {
        DISPLAY("Not enough memory, aborting\n");
        testResult = 1;
        goto _end;
    }
    RDG_genBuffer(CNBuffer, CNBuffSize, compressibility, 0., seed);

     
    DISPLAYLEVEL(3, "test%3i : ZSTD_getErrorName : ", testNb++);
    {   const char* errorString = ZSTD_getErrorName(0);
        DISPLAYLEVEL(3, "OK : %s \n", errorString);
    }

    DISPLAYLEVEL(3, "test%3i : ZSTD_getErrorName with wrong value : ", testNb++);
    {   const char* errorString = ZSTD_getErrorName(499);
        DISPLAYLEVEL(3, "OK : %s \n", errorString);
    }

    DISPLAYLEVEL(3, "test%3i : min compression level : ", testNb++);
    {   int const mcl = ZSTD_minCLevel();
        DISPLAYLEVEL(3, "%i (OK) \n", mcl);
    }

    DISPLAYLEVEL(3, "test%3i : compress %u bytes : ", testNb++, (U32)CNBuffSize);
    {   ZSTD_CCtx* const cctx = ZSTD_createCCtx();
        if (cctx==NULL) goto _output_error;
        CHECKPLUS(r, ZSTD_compressCCtx(cctx,
                            compressedBuffer, compressedBufferSize,
                            CNBuffer, CNBuffSize, 1),
                  cSize=r );
        DISPLAYLEVEL(3, "OK (%u bytes : %.2f%%)\n", (U32)cSize, (double)cSize/CNBuffSize*100);

        DISPLAYLEVEL(3, "test%3i : size of cctx for level 1 : ", testNb++);
        {   size_t const cctxSize = ZSTD_sizeof_CCtx(cctx);
            DISPLAYLEVEL(3, "%u bytes \n", (U32)cctxSize);
        }
        ZSTD_freeCCtx(cctx);
    }


    DISPLAYLEVEL(3, "test%3i : ZSTD_getFrameContentSize test : ", testNb++);
    {   unsigned long long const rSize = ZSTD_getFrameContentSize(compressedBuffer, cSize);
        if (rSize != CNBuffSize) goto _output_error;
    }
    DISPLAYLEVEL(3, "OK \n");

    DISPLAYLEVEL(3, "test%3i : ZSTD_findDecompressedSize test : ", testNb++);
    {   unsigned long long const rSize = ZSTD_findDecompressedSize(compressedBuffer, cSize);
        if (rSize != CNBuffSize) goto _output_error;
    }
    DISPLAYLEVEL(3, "OK \n");

    DISPLAYLEVEL(3, "test%3i : decompress %u bytes : ", testNb++, (U32)CNBuffSize);
    { size_t const r = ZSTD_decompress(decodedBuffer, CNBuffSize, compressedBuffer, cSize);
      if (r != CNBuffSize) goto _output_error; }
    DISPLAYLEVEL(3, "OK \n");

    DISPLAYLEVEL(3, "test%3i : check decompressed result : ", testNb++);
    {   size_t u;
        for (u=0; u<CNBuffSize; u++) {
            if (((BYTE*)decodedBuffer)[u] != ((BYTE*)CNBuffer)[u]) goto _output_error;;
    }   }
    DISPLAYLEVEL(3, "OK \n");


    DISPLAYLEVEL(3, "test%3i : decompress with null dict : ", testNb++);
    { size_t const r = ZSTD_decompress_usingDict(dctx, decodedBuffer, CNBuffSize, compressedBuffer, cSize, NULL, 0);
      if (r != CNBuffSize) goto _output_error; }
    DISPLAYLEVEL(3, "OK \n");

    DISPLAYLEVEL(3, "test%3i : decompress with null DDict : ", testNb++);
    { size_t const r = ZSTD_decompress_usingDDict(dctx, decodedBuffer, CNBuffSize, compressedBuffer, cSize, NULL);
      if (r != CNBuffSize) goto _output_error; }
    DISPLAYLEVEL(3, "OK \n");

    DISPLAYLEVEL(3, "test%3i : decompress with 1 missing byte : ", testNb++);
    { size_t const r = ZSTD_decompress(decodedBuffer, CNBuffSize, compressedBuffer, cSize-1);
      if (!ZSTD_isError(r)) goto _output_error;
      if (ZSTD_getErrorCode((size_t)r) != ZSTD_error_srcSize_wrong) goto _output_error; }
    DISPLAYLEVEL(3, "OK \n");

    DISPLAYLEVEL(3, "test%3i : decompress with 1 too much byte : ", testNb++);
    { size_t const r = ZSTD_decompress(decodedBuffer, CNBuffSize, compressedBuffer, cSize+1);
      if (!ZSTD_isError(r)) goto _output_error;
      if (ZSTD_getErrorCode(r) != ZSTD_error_srcSize_wrong) goto _output_error; }
    DISPLAYLEVEL(3, "OK \n");

    DISPLAYLEVEL(3, "test%3i : decompress too large input : ", testNb++);
    { size_t const r = ZSTD_decompress(decodedBuffer, CNBuffSize, compressedBuffer, compressedBufferSize);
      if (!ZSTD_isError(r)) goto _output_error;
      if (ZSTD_getErrorCode(r) != ZSTD_error_srcSize_wrong) goto _output_error; }
    DISPLAYLEVEL(3, "OK \n");

    DISPLAYLEVEL(3, "test%3d : check CCtx size after compressing empty input : ", testNb++);
    {   ZSTD_CCtx* const cctx = ZSTD_createCCtx();
        size_t const r = ZSTD_compressCCtx(cctx, compressedBuffer, compressedBufferSize, NULL, 0, 19);
        if (ZSTD_isError(r)) goto _output_error;
        if (ZSTD_sizeof_CCtx(cctx) > (1U << 20)) goto _output_error;
        ZSTD_freeCCtx(cctx);
        cSize = r;
    }
    DISPLAYLEVEL(3, "OK \n");

    DISPLAYLEVEL(3, "test%3d : decompress empty frame into NULL : ", testNb++);
    {   size_t const r = ZSTD_decompress(NULL, 0, compressedBuffer, cSize);
        if (ZSTD_isError(r)) goto _output_error;
        if (r != 0) goto _output_error;
    }
    {   ZSTD_CCtx* const cctx = ZSTD_createCCtx();
        ZSTD_outBuffer output;
        if (cctx==NULL) goto _output_error;
        output.dst = compressedBuffer;
        output.size = compressedBufferSize;
        output.pos = 0;
        CHECK_Z( ZSTD_initCStream(cctx, 1) );     
        CHECK_Z( ZSTD_flushStream(cctx, &output) );    
        CHECK_Z( ZSTD_endStream(cctx, &output) );
        ZSTD_freeCCtx(cctx);
         
        {   size_t const r = ZSTD_decompress(NULL, 0, compressedBuffer, output.pos);
            if (ZSTD_isError(r)) goto _output_error;
            if (r != 0) goto _output_error;
        }
         
        {   ZSTD_DCtx* const dstream = ZSTD_createDStream();
            ZSTD_inBuffer dinput;
            ZSTD_outBuffer doutput;
            size_t ipos;
            if (dstream==NULL) goto _output_error;
            dinput.src = compressedBuffer;
            dinput.size = 0;
            dinput.pos = 0;
            doutput.dst = NULL;
            doutput.size = 0;
            doutput.pos = 0;
            CHECK_Z ( ZSTD_initDStream(dstream) );
            for (ipos=1; ipos<=output.pos; ipos++) {
                dinput.size = ipos;
                CHECK_Z ( ZSTD_decompressStream(dstream, &doutput, &dinput) );
            }
            if (doutput.pos != 0) goto _output_error;
            ZSTD_freeDStream(dstream);
        }
    }
    DISPLAYLEVEL(3, "OK \n");

    DISPLAYLEVEL(3, "test%3d : re-use CCtx with expanding block size : ", testNb++);
    {   ZSTD_CCtx* const cctx = ZSTD_createCCtx();
        ZSTD_parameters const params = ZSTD_getParams(1, ZSTD_CONTENTSIZE_UNKNOWN, 0);
        assert(params.fParams.contentSizeFlag == 1);   
        CHECK_Z( ZSTD_compressBegin_advanced(cctx, NULL, 0, params, 1  ) );
        CHECK_Z( ZSTD_compressEnd(cctx, compressedBuffer, compressedBufferSize, CNBuffer, 1) );  

        CHECK_Z( ZSTD_compressBegin_advanced(cctx, NULL, 0, params, ZSTD_CONTENTSIZE_UNKNOWN) );   
        {   size_t const inSize = 2* 128 KB;
            size_t const outSize = ZSTD_compressBound(inSize);
            CHECK_Z( ZSTD_compressEnd(cctx, compressedBuffer, outSize, CNBuffer, inSize) );
             
        }
        ZSTD_freeCCtx(cctx);
    }
    DISPLAYLEVEL(3, "OK \n");

    DISPLAYLEVEL(3, "test%3d : re-using a CCtx should compress the same : ", testNb++);
    {   int i;
        for (i=0; i<20; i++)
            ((char*)CNBuffer)[i] = (char)i;    
        memcpy((char*)CNBuffer + 20, CNBuffer, 10);    
        for (i=1; i<=19; i++) {
            ZSTD_CCtx* const cctx = ZSTD_createCCtx();
            size_t size1, size2;
            DISPLAYLEVEL(5, "l%i ", i);
            size1 = ZSTD_compressCCtx(cctx, compressedBuffer, compressedBufferSize, CNBuffer, 30, i);
            CHECK_Z(size1);
            size2 = ZSTD_compressCCtx(cctx, compressedBuffer, compressedBufferSize, CNBuffer, 30, i);
            CHECK_Z(size2);
            CHECK_EQ(size1, size2);

            ZSTD_freeCCtx(cctx);
        }
    }
    DISPLAYLEVEL(3, "OK \n");

    DISPLAYLEVEL(3, "test%3d : ZSTD_CCtx_getParameter() : ", testNb++);
    {   ZSTD_CCtx* const cctx = ZSTD_createCCtx();
        ZSTD_outBuffer out = {NULL, 0, 0};
        ZSTD_inBuffer in = {NULL, 0, 0};
        unsigned value;

        CHECK_Z(ZSTD_CCtx_getParameter(cctx, ZSTD_p_compressionLevel, &value));
        CHECK_EQ(value, 3);
        CHECK_Z(ZSTD_CCtx_getParameter(cctx, ZSTD_p_hashLog, &value));
        CHECK_EQ(value, 0);
        CHECK_Z(ZSTD_CCtx_setParameter(cctx, ZSTD_p_hashLog, ZSTD_HASHLOG_MIN));
        CHECK_Z(ZSTD_CCtx_getParameter(cctx, ZSTD_p_compressionLevel, &value));
        CHECK_EQ(value, 3);
        CHECK_Z(ZSTD_CCtx_getParameter(cctx, ZSTD_p_hashLog, &value));
        CHECK_EQ(value, ZSTD_HASHLOG_MIN);
        CHECK_Z(ZSTD_CCtx_setParameter(cctx, ZSTD_p_compressionLevel, 7));
        CHECK_Z(ZSTD_CCtx_getParameter(cctx, ZSTD_p_compressionLevel, &value));
        CHECK_EQ(value, 7);
        CHECK_Z(ZSTD_CCtx_getParameter(cctx, ZSTD_p_hashLog, &value));
        CHECK_EQ(value, ZSTD_HASHLOG_MIN);
         
        ZSTD_compress_generic(cctx, &out, &in, ZSTD_e_continue);
        CHECK_Z(ZSTD_CCtx_getParameter(cctx, ZSTD_p_compressionLevel, &value));
        CHECK_EQ(value, 7);
        CHECK_Z(ZSTD_CCtx_getParameter(cctx, ZSTD_p_hashLog, &value));
        CHECK_EQ(value, ZSTD_HASHLOG_MIN);
         
        ZSTD_CCtx_reset(cctx);
        CHECK_Z(ZSTD_CCtx_getParameter(cctx, ZSTD_p_compressionLevel, &value));
        CHECK_EQ(value, 7);
        CHECK_Z(ZSTD_CCtx_getParameter(cctx, ZSTD_p_hashLog, &value));
        CHECK_EQ(value, ZSTD_HASHLOG_MIN);
         
        ZSTD_CCtx_resetParameters(cctx);
        CHECK_Z(ZSTD_CCtx_getParameter(cctx, ZSTD_p_compressionLevel, &value));
        CHECK_EQ(value, 3);
        CHECK_Z(ZSTD_CCtx_getParameter(cctx, ZSTD_p_hashLog, &value));
        CHECK_EQ(value, 0);

        ZSTD_freeCCtx(cctx);
    }
    DISPLAYLEVEL(3, "OK \n");

     
    DISPLAYLEVEL(3, "test%3d : overflow protection with large windowLog : ", testNb++);
    {   ZSTD_CCtx* const cctx = ZSTD_createCCtx();
        ZSTD_parameters params = ZSTD_getParams(-9, ZSTD_CONTENTSIZE_UNKNOWN, 0);
        size_t const nbCompressions = ((1U << 31) / CNBuffSize) + 1;    
        size_t cnb;
        assert(cctx != NULL);
        params.fParams.contentSizeFlag = 0;
        params.cParams.windowLog = ZSTD_WINDOWLOG_MAX;
        for (cnb = 0; cnb < nbCompressions; ++cnb) {
            DISPLAYLEVEL(6, "run %zu / %zu \n", cnb, nbCompressions);
            CHECK_Z( ZSTD_compressBegin_advanced(cctx, NULL, 0, params, ZSTD_CONTENTSIZE_UNKNOWN) );   
            CHECK_Z( ZSTD_compressEnd(cctx, compressedBuffer, compressedBufferSize, CNBuffer, CNBuffSize) );
        }
        ZSTD_freeCCtx(cctx);
    }
    DISPLAYLEVEL(3, "OK \n");

    DISPLAYLEVEL(3, "test%3d : size down context : ", testNb++);
    {   ZSTD_CCtx* const largeCCtx = ZSTD_createCCtx();
        assert(largeCCtx != NULL);
        CHECK_Z( ZSTD_compressBegin(largeCCtx, 19) );    
        CHECK_Z( ZSTD_compressEnd(largeCCtx, compressedBuffer, compressedBufferSize, CNBuffer, 1) );
        {   size_t const largeCCtxSize = ZSTD_sizeof_CCtx(largeCCtx);    
            {   ZSTD_CCtx* const smallCCtx = ZSTD_createCCtx();
                assert(smallCCtx != NULL);
                CHECK_Z(ZSTD_compressCCtx(smallCCtx, compressedBuffer, compressedBufferSize, CNBuffer, 1, 1));
                {   size_t const smallCCtxSize = ZSTD_sizeof_CCtx(smallCCtx);
                    DISPLAYLEVEL(5, "(large) %zuKB > 32*%zuKB (small) : ",
                                largeCCtxSize>>10, smallCCtxSize>>10);
                    assert(largeCCtxSize > 32* smallCCtxSize);   
                }
                ZSTD_freeCCtx(smallCCtx);
            }
            {   U32 const maxNbAttempts = 1100;    
                U32 u;
                for (u=0; u<maxNbAttempts; u++) {
                    CHECK_Z(ZSTD_compressCCtx(largeCCtx, compressedBuffer, compressedBufferSize, CNBuffer, 1, 1));
                    if (ZSTD_sizeof_CCtx(largeCCtx) < largeCCtxSize) break;    
                }
                DISPLAYLEVEL(5, "size down after %u attempts : ", u);
                if (u==maxNbAttempts) goto _output_error;    
            }
        }
        ZSTD_freeCCtx(largeCCtx);
    }
    DISPLAYLEVEL(3, "OK \n");

     
#define STATIC_CCTX_LEVEL 3
    DISPLAYLEVEL(3, "test%3i : create static CCtx for level %u :", testNb++, STATIC_CCTX_LEVEL);
    {   size_t const staticCCtxSize = ZSTD_estimateCStreamSize(STATIC_CCTX_LEVEL);
        void* const staticCCtxBuffer = malloc(staticCCtxSize);
        size_t const staticDCtxSize = ZSTD_estimateDCtxSize();
        void* const staticDCtxBuffer = malloc(staticDCtxSize);
        if (staticCCtxBuffer==NULL || staticDCtxBuffer==NULL) {
            free(staticCCtxBuffer);
            free(staticDCtxBuffer);
            DISPLAY("Not enough memory, aborting\n");
            testResult = 1;
            goto _end;
        }
        {   ZSTD_CCtx* staticCCtx = ZSTD_initStaticCCtx(staticCCtxBuffer, staticCCtxSize);
            ZSTD_DCtx* staticDCtx = ZSTD_initStaticDCtx(staticDCtxBuffer, staticDCtxSize);
            if ((staticCCtx==NULL) || (staticDCtx==NULL)) goto _output_error;
            DISPLAYLEVEL(3, "OK \n");

            DISPLAYLEVEL(3, "test%3i : init CCtx for level %u : ", testNb++, STATIC_CCTX_LEVEL);
            { size_t const r = ZSTD_compressBegin(staticCCtx, STATIC_CCTX_LEVEL);
              if (ZSTD_isError(r)) goto _output_error; }
            DISPLAYLEVEL(3, "OK \n");

            DISPLAYLEVEL(3, "test%3i : simple compression test with static CCtx : ", testNb++);
            CHECKPLUS(r, ZSTD_compressCCtx(staticCCtx,
                            compressedBuffer, compressedBufferSize,
                            CNBuffer, CNBuffSize, STATIC_CCTX_LEVEL),
                      cSize=r );
            DISPLAYLEVEL(3, "OK (%u bytes : %.2f%%)\n",
                            (U32)cSize, (double)cSize/CNBuffSize*100);

            DISPLAYLEVEL(3, "test%3i : simple decompression test with static DCtx : ", testNb++);
            { size_t const r = ZSTD_decompressDCtx(staticDCtx,
                                                decodedBuffer, CNBuffSize,
                                                compressedBuffer, cSize);
              if (r != CNBuffSize) goto _output_error; }
            DISPLAYLEVEL(3, "OK \n");

            DISPLAYLEVEL(3, "test%3i : check decompressed result : ", testNb++);
            {   size_t u;
                for (u=0; u<CNBuffSize; u++) {
                    if (((BYTE*)decodedBuffer)[u] != ((BYTE*)CNBuffer)[u])
                        goto _output_error;;
            }   }
            DISPLAYLEVEL(3, "OK \n");

            DISPLAYLEVEL(3, "test%3i : init CCtx for too large level (must fail) : ", testNb++);
            { size_t const r = ZSTD_compressBegin(staticCCtx, ZSTD_maxCLevel());
              if (!ZSTD_isError(r)) goto _output_error; }
            DISPLAYLEVEL(3, "OK \n");

            DISPLAYLEVEL(3, "test%3i : init CCtx for small level %u (should work again) : ", testNb++, 1);
            { size_t const r = ZSTD_compressBegin(staticCCtx, 1);
              if (ZSTD_isError(r)) goto _output_error; }
            DISPLAYLEVEL(3, "OK \n");

            DISPLAYLEVEL(3, "test%3i : init CStream for small level %u : ", testNb++, 1);
            { size_t const r = ZSTD_initCStream(staticCCtx, 1);
              if (ZSTD_isError(r)) goto _output_error; }
            DISPLAYLEVEL(3, "OK \n");

            DISPLAYLEVEL(3, "test%3i : init CStream with dictionary (should fail) : ", testNb++);
            { size_t const r = ZSTD_initCStream_usingDict(staticCCtx, CNBuffer, 64 KB, 1);
              if (!ZSTD_isError(r)) goto _output_error; }
            DISPLAYLEVEL(3, "OK \n");

            DISPLAYLEVEL(3, "test%3i : init DStream (should fail) : ", testNb++);
            { size_t const r = ZSTD_initDStream(staticDCtx);
              if (ZSTD_isError(r)) goto _output_error; }
            {   ZSTD_outBuffer output = { decodedBuffer, CNBuffSize, 0 };
                ZSTD_inBuffer input = { compressedBuffer, ZSTD_FRAMEHEADERSIZE_MAX+1, 0 };
                size_t const r = ZSTD_decompressStream(staticDCtx, &output, &input);
                if (!ZSTD_isError(r)) goto _output_error;
            }
            DISPLAYLEVEL(3, "OK \n");
        }
        free(staticCCtxBuffer);
        free(staticDCtxBuffer);
    }


     
    DISPLAYLEVEL(3, "test%3i : create ZSTDMT CCtx : ", testNb++);
    {   ZSTDMT_CCtx* mtctx = ZSTDMT_createCCtx(2);
        if (mtctx==NULL) {
            DISPLAY("mtctx : mot enough memory, aborting \n");
            testResult = 1;
            goto _end;
        }
        DISPLAYLEVEL(3, "OK \n");

        DISPLAYLEVEL(3, "test%3i : compress %u bytes with 2 threads : ", testNb++, (U32)CNBuffSize);
        CHECKPLUS(r, ZSTDMT_compressCCtx(mtctx,
                                compressedBuffer, compressedBufferSize,
                                CNBuffer, CNBuffSize,
                                1),
                  cSize=r );
        DISPLAYLEVEL(3, "OK (%u bytes : %.2f%%)\n", (U32)cSize, (double)cSize/CNBuffSize*100);

        DISPLAYLEVEL(3, "test%3i : decompressed size test : ", testNb++);
        {   unsigned long long const rSize = ZSTD_getFrameContentSize(compressedBuffer, cSize);
            if (rSize != CNBuffSize)  {
                DISPLAY("ZSTD_getFrameContentSize incorrect : %u != %u \n", (U32)rSize, (U32)CNBuffSize);
                goto _output_error;
        }   }
        DISPLAYLEVEL(3, "OK \n");

        DISPLAYLEVEL(3, "test%3i : decompress %u bytes : ", testNb++, (U32)CNBuffSize);
        { size_t const r = ZSTD_decompress(decodedBuffer, CNBuffSize, compressedBuffer, cSize);
          if (r != CNBuffSize) goto _output_error; }
        DISPLAYLEVEL(3, "OK \n");

        DISPLAYLEVEL(3, "test%3i : check decompressed result : ", testNb++);
        {   size_t u;
            for (u=0; u<CNBuffSize; u++) {
                if (((BYTE*)decodedBuffer)[u] != ((BYTE*)CNBuffer)[u]) goto _output_error;;
        }   }
        DISPLAYLEVEL(3, "OK \n");

        DISPLAYLEVEL(3, "test%3i : compress -T2 with checksum : ", testNb++);
        {   ZSTD_parameters params = ZSTD_getParams(1, CNBuffSize, 0);
            params.fParams.checksumFlag = 1;
            params.fParams.contentSizeFlag = 1;
            CHECKPLUS(r, ZSTDMT_compress_advanced(mtctx,
                                    compressedBuffer, compressedBufferSize,
                                    CNBuffer, CNBuffSize,
                                    NULL, params, 3  ),
                      cSize=r );
        }
        DISPLAYLEVEL(3, "OK (%u bytes : %.2f%%)\n", (U32)cSize, (double)cSize/CNBuffSize*100);

        DISPLAYLEVEL(3, "test%3i : decompress %u bytes : ", testNb++, (U32)CNBuffSize);
        { size_t const r = ZSTD_decompress(decodedBuffer, CNBuffSize, compressedBuffer, cSize);
          if (r != CNBuffSize) goto _output_error; }
        DISPLAYLEVEL(3, "OK \n");

        ZSTDMT_freeCCtx(mtctx);
    }


     
    DISPLAYLEVEL(3, "test%3i : compress multiple frames : ", testNb++);
    {   size_t off = 0;
        int i;
        int const segs = 4;
         
        size_t const segSize = (CNBuffSize / 2) / segs;
        for (i = 0; i < segs; i++) {
            CHECK_V(r, ZSTD_compress(
                            (BYTE *)compressedBuffer + off, CNBuffSize - off,
                            (BYTE *)CNBuffer + segSize * i,
                            segSize, 5));
            off += r;
            if (i == segs/2) {
                 
                const U32 skipLen = 129 KB;
                MEM_writeLE32((BYTE*)compressedBuffer + off, ZSTD_MAGIC_SKIPPABLE_START);
                MEM_writeLE32((BYTE*)compressedBuffer + off + 4, skipLen);
                off += skipLen + ZSTD_skippableHeaderSize;
            }
        }
        cSize = off;
    }
    DISPLAYLEVEL(3, "OK \n");

    DISPLAYLEVEL(3, "test%3i : get decompressed size of multiple frames : ", testNb++);
    {   unsigned long long const r = ZSTD_findDecompressedSize(compressedBuffer, cSize);
        if (r != CNBuffSize / 2) goto _output_error; }
    DISPLAYLEVEL(3, "OK \n");

    DISPLAYLEVEL(3, "test%3i : decompress multiple frames : ", testNb++);
    {   CHECK_V(r, ZSTD_decompress(decodedBuffer, CNBuffSize, compressedBuffer, cSize));
        if (r != CNBuffSize / 2) goto _output_error; }
    DISPLAYLEVEL(3, "OK \n");

    DISPLAYLEVEL(3, "test%3i : check decompressed result : ", testNb++);
    if (memcmp(decodedBuffer, CNBuffer, CNBuffSize / 2) != 0) goto _output_error;
    DISPLAYLEVEL(3, "OK \n");

     
    {   ZSTD_CCtx* const ctxOrig = ZSTD_createCCtx();
        ZSTD_CCtx* const ctxDuplicated = ZSTD_createCCtx();
        static const size_t dictSize = 551;

        DISPLAYLEVEL(3, "test%3i : copy context too soon : ", testNb++);
        { size_t const copyResult = ZSTD_copyCCtx(ctxDuplicated, ctxOrig, 0);
          if (!ZSTD_isError(copyResult)) goto _output_error; }    
        DISPLAYLEVEL(3, "OK \n");

        DISPLAYLEVEL(3, "test%3i : load dictionary into context : ", testNb++);
        CHECK( ZSTD_compressBegin_usingDict(ctxOrig, CNBuffer, dictSize, 2) );
        CHECK( ZSTD_copyCCtx(ctxDuplicated, ctxOrig, 0) );  
        DISPLAYLEVEL(3, "OK \n");

        DISPLAYLEVEL(3, "test%3i : compress with flat dictionary : ", testNb++);
        cSize = 0;
        CHECKPLUS(r, ZSTD_compressEnd(ctxOrig, compressedBuffer, compressedBufferSize,
                                           (const char*)CNBuffer + dictSize, CNBuffSize - dictSize),
                  cSize += r);
        DISPLAYLEVEL(3, "OK (%u bytes : %.2f%%)\n", (U32)cSize, (double)cSize/CNBuffSize*100);

        DISPLAYLEVEL(3, "test%3i : frame built with flat dictionary should be decompressible : ", testNb++);
        CHECKPLUS(r, ZSTD_decompress_usingDict(dctx,
                                       decodedBuffer, CNBuffSize,
                                       compressedBuffer, cSize,
                                       CNBuffer, dictSize),
                  if (r != CNBuffSize - dictSize) goto _output_error);
        DISPLAYLEVEL(3, "OK \n");

        DISPLAYLEVEL(3, "test%3i : compress with duplicated context : ", testNb++);
        {   size_t const cSizeOrig = cSize;
            cSize = 0;
            CHECKPLUS(r, ZSTD_compressEnd(ctxDuplicated, compressedBuffer, compressedBufferSize,
                                               (const char*)CNBuffer + dictSize, CNBuffSize - dictSize),
                      cSize += r);
            if (cSize != cSizeOrig) goto _output_error;    
        }
        DISPLAYLEVEL(3, "OK (%u bytes : %.2f%%)\n", (U32)cSize, (double)cSize/CNBuffSize*100);

        DISPLAYLEVEL(3, "test%3i : frame built with duplicated context should be decompressible : ", testNb++);
        CHECKPLUS(r, ZSTD_decompress_usingDict(dctx,
                                           decodedBuffer, CNBuffSize,
                                           compressedBuffer, cSize,
                                           CNBuffer, dictSize),
                  if (r != CNBuffSize - dictSize) goto _output_error);
        DISPLAYLEVEL(3, "OK \n");

        DISPLAYLEVEL(3, "test%3i : decompress with DDict : ", testNb++);
        {   ZSTD_DDict* const ddict = ZSTD_createDDict(CNBuffer, dictSize);
            size_t const r = ZSTD_decompress_usingDDict(dctx, decodedBuffer, CNBuffSize, compressedBuffer, cSize, ddict);
            if (r != CNBuffSize - dictSize) goto _output_error;
            DISPLAYLEVEL(3, "OK (size of DDict : %u) \n", (U32)ZSTD_sizeof_DDict(ddict));
            ZSTD_freeDDict(ddict);
        }

        DISPLAYLEVEL(3, "test%3i : decompress with static DDict : ", testNb++);
        {   size_t const ddictBufferSize = ZSTD_estimateDDictSize(dictSize, ZSTD_dlm_byCopy);
            void* ddictBuffer = malloc(ddictBufferSize);
            if (ddictBuffer == NULL) goto _output_error;
            {   const ZSTD_DDict* const ddict = ZSTD_initStaticDDict(ddictBuffer, ddictBufferSize, CNBuffer, dictSize, ZSTD_dlm_byCopy, ZSTD_dct_auto);
                size_t const r = ZSTD_decompress_usingDDict(dctx, decodedBuffer, CNBuffSize, compressedBuffer, cSize, ddict);
                if (r != CNBuffSize - dictSize) goto _output_error;
            }
            free(ddictBuffer);
            DISPLAYLEVEL(3, "OK (size of static DDict : %u) \n", (U32)ddictBufferSize);
        }

        DISPLAYLEVEL(3, "test%3i : check content size on duplicated context : ", testNb++);
        {   size_t const testSize = CNBuffSize / 3;
            {   ZSTD_parameters p = ZSTD_getParams(2, testSize, dictSize);
                p.fParams.contentSizeFlag = 1;
                CHECK( ZSTD_compressBegin_advanced(ctxOrig, CNBuffer, dictSize, p, testSize-1) );
            }
            CHECK( ZSTD_copyCCtx(ctxDuplicated, ctxOrig, testSize) );

            CHECKPLUS(r, ZSTD_compressEnd(ctxDuplicated, compressedBuffer, ZSTD_compressBound(testSize),
                                          (const char*)CNBuffer + dictSize, testSize),
                      cSize = r);
            {   ZSTD_frameHeader zfh;
                if (ZSTD_getFrameHeader(&zfh, compressedBuffer, cSize)) goto _output_error;
                if ((zfh.frameContentSize != testSize) && (zfh.frameContentSize != 0)) goto _output_error;
        }   }
        DISPLAYLEVEL(3, "OK \n");

        ZSTD_freeCCtx(ctxOrig);
        ZSTD_freeCCtx(ctxDuplicated);
    }

     
    {   ZSTD_CCtx* const cctx = ZSTD_createCCtx();
        size_t const dictBufferCapacity = 16 KB;
        void* dictBuffer = malloc(dictBufferCapacity);
        size_t const totalSampleSize = 1 MB;
        size_t const sampleUnitSize = 8 KB;
        U32 const nbSamples = (U32)(totalSampleSize / sampleUnitSize);
        size_t* const samplesSizes = (size_t*) malloc(nbSamples * sizeof(size_t));
        size_t dictSize;
        U32 dictID;

        if (dictBuffer==NULL || samplesSizes==NULL) {
            free(dictBuffer);
            free(samplesSizes);
            goto _output_error;
        }

        DISPLAYLEVEL(3, "test%3i : dictBuilder on cyclic data : ", testNb++);
        assert(compressedBufferSize >= totalSampleSize);
        { U32 u; for (u=0; u<totalSampleSize; u++) ((BYTE*)decodedBuffer)[u] = (BYTE)u; }
        { U32 u; for (u=0; u<nbSamples; u++) samplesSizes[u] = sampleUnitSize; }
        {   size_t const sDictSize = ZDICT_trainFromBuffer(dictBuffer, dictBufferCapacity,
                                         decodedBuffer, samplesSizes, nbSamples);
            if (ZDICT_isError(sDictSize)) goto _output_error;
            DISPLAYLEVEL(3, "OK, created dictionary of size %u \n", (U32)sDictSize);
        }

        DISPLAYLEVEL(3, "test%3i : dictBuilder : ", testNb++);
        { U32 u; for (u=0; u<nbSamples; u++) samplesSizes[u] = sampleUnitSize; }
        dictSize = ZDICT_trainFromBuffer(dictBuffer, dictBufferCapacity,
                                         CNBuffer, samplesSizes, nbSamples);
        if (ZDICT_isError(dictSize)) goto _output_error;
        DISPLAYLEVEL(3, "OK, created dictionary of size %u \n", (U32)dictSize);

        DISPLAYLEVEL(3, "test%3i : check dictID : ", testNb++);
        dictID = ZDICT_getDictID(dictBuffer, dictSize);
        if (dictID==0) goto _output_error;
        DISPLAYLEVEL(3, "OK : %u \n", dictID);

        DISPLAYLEVEL(3, "test%3i : compress with dictionary : ", testNb++);
        cSize = ZSTD_compress_usingDict(cctx, compressedBuffer, compressedBufferSize,
                                        CNBuffer, CNBuffSize,
                                        dictBuffer, dictSize, 4);
        if (ZSTD_isError(cSize)) goto _output_error;
        DISPLAYLEVEL(3, "OK (%u bytes : %.2f%%)\n", (U32)cSize, (double)cSize/CNBuffSize*100);

        DISPLAYLEVEL(3, "test%3i : retrieve dictID from dictionary : ", testNb++);
        {   U32 const did = ZSTD_getDictID_fromDict(dictBuffer, dictSize);
            if (did != dictID) goto _output_error;    
        }
        DISPLAYLEVEL(3, "OK \n");

        DISPLAYLEVEL(3, "test%3i : retrieve dictID from frame : ", testNb++);
        {   U32 const did = ZSTD_getDictID_fromFrame(compressedBuffer, cSize);
            if (did != dictID) goto _output_error;    
        }
        DISPLAYLEVEL(3, "OK \n");

        DISPLAYLEVEL(3, "test%3i : frame built with dictionary should be decompressible : ", testNb++);
        CHECKPLUS(r, ZSTD_decompress_usingDict(dctx,
                                       decodedBuffer, CNBuffSize,
                                       compressedBuffer, cSize,
                                       dictBuffer, dictSize),
                  if (r != CNBuffSize) goto _output_error);
        DISPLAYLEVEL(3, "OK \n");

        DISPLAYLEVEL(3, "test%3i : estimate CDict size : ", testNb++);
        {   ZSTD_compressionParameters const cParams = ZSTD_getCParams(1, CNBuffSize, dictSize);
            size_t const estimatedSize = ZSTD_estimateCDictSize_advanced(dictSize, cParams, ZSTD_dlm_byRef);
            DISPLAYLEVEL(3, "OK : %u \n", (U32)estimatedSize);
        }

        DISPLAYLEVEL(3, "test%3i : compress with CDict ", testNb++);
        {   ZSTD_compressionParameters const cParams = ZSTD_getCParams(1, CNBuffSize, dictSize);
            ZSTD_CDict* const cdict = ZSTD_createCDict_advanced(dictBuffer, dictSize,
                                            ZSTD_dlm_byRef, ZSTD_dct_auto,
                                            cParams, ZSTD_defaultCMem);
            DISPLAYLEVEL(3, "(size : %u) : ", (U32)ZSTD_sizeof_CDict(cdict));
            cSize = ZSTD_compress_usingCDict(cctx, compressedBuffer, compressedBufferSize,
                                                 CNBuffer, CNBuffSize, cdict);
            ZSTD_freeCDict(cdict);
            if (ZSTD_isError(cSize)) goto _output_error;
        }
        DISPLAYLEVEL(3, "OK (%u bytes : %.2f%%)\n", (U32)cSize, (double)cSize/CNBuffSize*100);

        DISPLAYLEVEL(3, "test%3i : retrieve dictID from frame : ", testNb++);
        {   U32 const did = ZSTD_getDictID_fromFrame(compressedBuffer, cSize);
            if (did != dictID) goto _output_error;    
        }
        DISPLAYLEVEL(3, "OK \n");

        DISPLAYLEVEL(3, "test%3i : frame built with dictionary should be decompressible : ", testNb++);
        CHECKPLUS(r, ZSTD_decompress_usingDict(dctx,
                                       decodedBuffer, CNBuffSize,
                                       compressedBuffer, cSize,
                                       dictBuffer, dictSize),
                  if (r != CNBuffSize) goto _output_error);
        DISPLAYLEVEL(3, "OK \n");

        DISPLAYLEVEL(3, "test%3i : compress with static CDict : ", testNb++);
        {   int const maxLevel = ZSTD_maxCLevel();
            int level;
            for (level = 1; level <= maxLevel; ++level) {
                ZSTD_compressionParameters const cParams = ZSTD_getCParams(level, CNBuffSize, dictSize);
                size_t const cdictSize = ZSTD_estimateCDictSize_advanced(dictSize, cParams, ZSTD_dlm_byCopy);
                void* const cdictBuffer = malloc(cdictSize);
                if (cdictBuffer==NULL) goto _output_error;
                {   const ZSTD_CDict* const cdict = ZSTD_initStaticCDict(
                                                cdictBuffer, cdictSize,
                                                dictBuffer, dictSize,
                                                ZSTD_dlm_byCopy, ZSTD_dct_auto,
                                                cParams);
                    if (cdict == NULL) {
                        DISPLAY("ZSTD_initStaticCDict failed ");
                        goto _output_error;
                    }
                    cSize = ZSTD_compress_usingCDict(cctx,
                                    compressedBuffer, compressedBufferSize,
                                    CNBuffer, MIN(10 KB, CNBuffSize), cdict);
                    if (ZSTD_isError(cSize)) {
                        DISPLAY("ZSTD_compress_usingCDict failed ");
                        goto _output_error;
                }   }
                free(cdictBuffer);
        }   }
        DISPLAYLEVEL(3, "OK (%u bytes : %.2f%%)\n", (U32)cSize, (double)cSize/CNBuffSize*100);

        DISPLAYLEVEL(3, "test%3i : ZSTD_compress_usingCDict_advanced, no contentSize, no dictID : ", testNb++);
        {   ZSTD_frameParameters const fParams = { 0  , 1  , 1   };
            ZSTD_compressionParameters const cParams = ZSTD_getCParams(1, CNBuffSize, dictSize);
            ZSTD_CDict* const cdict = ZSTD_createCDict_advanced(dictBuffer, dictSize, ZSTD_dlm_byRef, ZSTD_dct_auto, cParams, ZSTD_defaultCMem);
            cSize = ZSTD_compress_usingCDict_advanced(cctx, compressedBuffer, compressedBufferSize,
                                                 CNBuffer, CNBuffSize, cdict, fParams);
            ZSTD_freeCDict(cdict);
            if (ZSTD_isError(cSize)) goto _output_error;
        }
        DISPLAYLEVEL(3, "OK (%u bytes : %.2f%%)\n", (U32)cSize, (double)cSize/CNBuffSize*100);

        DISPLAYLEVEL(3, "test%3i : try retrieving contentSize from frame : ", testNb++);
        {   U64 const contentSize = ZSTD_getFrameContentSize(compressedBuffer, cSize);
            if (contentSize != ZSTD_CONTENTSIZE_UNKNOWN) goto _output_error;
        }
        DISPLAYLEVEL(3, "OK (unknown)\n");

        DISPLAYLEVEL(3, "test%3i : frame built without dictID should be decompressible : ", testNb++);
        CHECKPLUS(r, ZSTD_decompress_usingDict(dctx,
                                       decodedBuffer, CNBuffSize,
                                       compressedBuffer, cSize,
                                       dictBuffer, dictSize),
                  if (r != CNBuffSize) goto _output_error);
        DISPLAYLEVEL(3, "OK \n");

        DISPLAYLEVEL(3, "test%3i : ZSTD_compress_advanced, no dictID : ", testNb++);
        {   ZSTD_parameters p = ZSTD_getParams(3, CNBuffSize, dictSize);
            p.fParams.noDictIDFlag = 1;
            cSize = ZSTD_compress_advanced(cctx, compressedBuffer, compressedBufferSize,
                                           CNBuffer, CNBuffSize,
                                           dictBuffer, dictSize, p);
            if (ZSTD_isError(cSize)) goto _output_error;
        }
        DISPLAYLEVEL(3, "OK (%u bytes : %.2f%%)\n", (U32)cSize, (double)cSize/CNBuffSize*100);

        DISPLAYLEVEL(3, "test%3i : frame built without dictID should be decompressible : ", testNb++);
        CHECKPLUS(r, ZSTD_decompress_usingDict(dctx,
                                       decodedBuffer, CNBuffSize,
                                       compressedBuffer, cSize,
                                       dictBuffer, dictSize),
                  if (r != CNBuffSize) goto _output_error);
        DISPLAYLEVEL(3, "OK \n");

        DISPLAYLEVEL(3, "test%3i : dictionary containing only header should return error : ", testNb++);
        {
          const size_t ret = ZSTD_decompress_usingDict(
              dctx, decodedBuffer, CNBuffSize, compressedBuffer, cSize,
              "\x37\xa4\x30\xec\x11\x22\x33\x44", 8);
          if (ZSTD_getErrorCode(ret) != ZSTD_error_dictionary_corrupted) goto _output_error;
        }
        DISPLAYLEVEL(3, "OK \n");

        DISPLAYLEVEL(3, "test%3i : Building cdict w/ ZSTD_dm_fullDict on a good dictionary : ", testNb++);
        {   ZSTD_compressionParameters const cParams = ZSTD_getCParams(1, CNBuffSize, dictSize);
            ZSTD_CDict* const cdict = ZSTD_createCDict_advanced(dictBuffer, dictSize, ZSTD_dlm_byRef, ZSTD_dct_fullDict, cParams, ZSTD_defaultCMem);
            if (cdict==NULL) goto _output_error;
            ZSTD_freeCDict(cdict);
        }
        DISPLAYLEVEL(3, "OK \n");

        DISPLAYLEVEL(3, "test%3i : Building cdict w/ ZSTD_dm_fullDict on a rawContent (must fail) : ", testNb++);
        {   ZSTD_compressionParameters const cParams = ZSTD_getCParams(1, CNBuffSize, dictSize);
            ZSTD_CDict* const cdict = ZSTD_createCDict_advanced((const char*)dictBuffer+1, dictSize-1, ZSTD_dlm_byRef, ZSTD_dct_fullDict, cParams, ZSTD_defaultCMem);
            if (cdict!=NULL) goto _output_error;
            ZSTD_freeCDict(cdict);
        }
        DISPLAYLEVEL(3, "OK \n");

        DISPLAYLEVEL(3, "test%3i : Loading rawContent starting with dict header w/ ZSTD_dm_auto should fail : ", testNb++);
        {
            size_t ret;
            MEM_writeLE32((char*)dictBuffer+2, ZSTD_MAGIC_DICTIONARY);
            ret = ZSTD_CCtx_loadDictionary_advanced(
                    cctx, (const char*)dictBuffer+2, dictSize-2, ZSTD_dlm_byRef, ZSTD_dct_auto);
            if (!ZSTD_isError(ret)) goto _output_error;
        }
        DISPLAYLEVEL(3, "OK \n");

        DISPLAYLEVEL(3, "test%3i : Loading rawContent starting with dict header w/ ZSTD_dm_rawContent should pass : ", testNb++);
        {
            size_t ret;
            MEM_writeLE32((char*)dictBuffer+2, ZSTD_MAGIC_DICTIONARY);
            ret = ZSTD_CCtx_loadDictionary_advanced(
                    cctx, (const char*)dictBuffer+2, dictSize-2, ZSTD_dlm_byRef, ZSTD_dct_rawContent);
            if (ZSTD_isError(ret)) goto _output_error;
        }
        DISPLAYLEVEL(3, "OK \n");

        DISPLAYLEVEL(3, "test%3i : Dictionary with non-default repcodes : ", testNb++);
        { U32 u; for (u=0; u<nbSamples; u++) samplesSizes[u] = sampleUnitSize; }
        dictSize = ZDICT_trainFromBuffer(dictBuffer, dictSize,
                                         CNBuffer, samplesSizes, nbSamples);
        if (ZDICT_isError(dictSize)) goto _output_error;
         
        {
            BYTE* dictPtr = (BYTE*)dictBuffer;
            BYTE* dictLimit = dictPtr + dictSize - 12;
             
            while (dictPtr < dictLimit &&
                   (MEM_readLE32(dictPtr) != 1 || MEM_readLE32(dictPtr + 4) != 4 ||
                    MEM_readLE32(dictPtr + 8) != 8)) {
                ++dictPtr;
            }
            if (dictPtr >= dictLimit) goto _output_error;
            MEM_writeLE32(dictPtr + 0, 10);
            MEM_writeLE32(dictPtr + 4, 10);
            MEM_writeLE32(dictPtr + 8, 10);
             
            memset((BYTE*)dictBuffer + dictSize - 8, 'x', 8);
        }
         
        {   size_t dSize;
            BYTE data[1024];
            ZSTD_compressionParameters const cParams = ZSTD_getCParams(19, CNBuffSize, dictSize);
            ZSTD_CDict* const cdict = ZSTD_createCDict_advanced(dictBuffer, dictSize,
                                            ZSTD_dlm_byRef, ZSTD_dct_auto,
                                            cParams, ZSTD_defaultCMem);
            memset(data, 'x', sizeof(data));
            cSize = ZSTD_compress_usingCDict(cctx, compressedBuffer, compressedBufferSize,
                                             data, sizeof(data), cdict);
            ZSTD_freeCDict(cdict);
            if (ZSTD_isError(cSize)) { DISPLAYLEVEL(5, "Compression error %s : ", ZSTD_getErrorName(cSize)); goto _output_error; }
            dSize = ZSTD_decompress_usingDict(dctx, decodedBuffer, sizeof(data), compressedBuffer, cSize, dictBuffer, dictSize);
            if (ZSTD_isError(dSize)) { DISPLAYLEVEL(5, "Decompression error %s : ", ZSTD_getErrorName(dSize)); goto _output_error; }
            if (memcmp(data, decodedBuffer, sizeof(data))) { DISPLAYLEVEL(5, "Data corruption : "); goto _output_error; }
        }
        DISPLAYLEVEL(3, "OK \n");

        ZSTD_freeCCtx(cctx);
        free(dictBuffer);
        free(samplesSizes);
    }

     
    {   ZSTD_CCtx* const cctx = ZSTD_createCCtx();
        size_t dictSize = 16 KB;
        size_t optDictSize = dictSize;
        void* dictBuffer = malloc(dictSize);
        size_t const totalSampleSize = 1 MB;
        size_t const sampleUnitSize = 8 KB;
        U32 const nbSamples = (U32)(totalSampleSize / sampleUnitSize);
        size_t* const samplesSizes = (size_t*) malloc(nbSamples * sizeof(size_t));
        ZDICT_cover_params_t params;
        U32 dictID;

        if (dictBuffer==NULL || samplesSizes==NULL) {
            free(dictBuffer);
            free(samplesSizes);
            goto _output_error;
        }

        DISPLAYLEVEL(3, "test%3i : ZDICT_trainFromBuffer_cover : ", testNb++);
        { U32 u; for (u=0; u<nbSamples; u++) samplesSizes[u] = sampleUnitSize; }
        memset(&params, 0, sizeof(params));
        params.d = 1 + (FUZ_rand(&seed) % 16);
        params.k = params.d + (FUZ_rand(&seed) % 256);
        dictSize = ZDICT_trainFromBuffer_cover(dictBuffer, dictSize,
                                               CNBuffer, samplesSizes, nbSamples,
                                               params);
        if (ZDICT_isError(dictSize)) goto _output_error;
        DISPLAYLEVEL(3, "OK, created dictionary of size %u \n", (U32)dictSize);

        DISPLAYLEVEL(3, "test%3i : check dictID : ", testNb++);
        dictID = ZDICT_getDictID(dictBuffer, dictSize);
        if (dictID==0) goto _output_error;
        DISPLAYLEVEL(3, "OK : %u \n", dictID);

        DISPLAYLEVEL(3, "test%3i : ZDICT_optimizeTrainFromBuffer_cover : ", testNb++);
        memset(&params, 0, sizeof(params));
        params.steps = 4;
        optDictSize = ZDICT_optimizeTrainFromBuffer_cover(dictBuffer, optDictSize,
                                                          CNBuffer, samplesSizes,
                                                          nbSamples / 4, &params);
        if (ZDICT_isError(optDictSize)) goto _output_error;
        DISPLAYLEVEL(3, "OK, created dictionary of size %u \n", (U32)optDictSize);

        DISPLAYLEVEL(3, "test%3i : check dictID : ", testNb++);
        dictID = ZDICT_getDictID(dictBuffer, optDictSize);
        if (dictID==0) goto _output_error;
        DISPLAYLEVEL(3, "OK : %u \n", dictID);

        ZSTD_freeCCtx(cctx);
        free(dictBuffer);
        free(samplesSizes);
    }

     
    DISPLAYLEVEL(3, "test%3i : Check input length for magic number : ", testNb++);
    { size_t const r = ZSTD_decompress(decodedBuffer, CNBuffSize, CNBuffer, 3);    
      if (!ZSTD_isError(r)) goto _output_error;
      if (ZSTD_getErrorCode(r) != ZSTD_error_srcSize_wrong) goto _output_error; }
    DISPLAYLEVEL(3, "OK \n");

    DISPLAYLEVEL(3, "test%3i : Check magic Number : ", testNb++);
    ((char*)(CNBuffer))[0] = 1;
    { size_t const r = ZSTD_decompress(decodedBuffer, CNBuffSize, CNBuffer, 4);
      if (!ZSTD_isError(r)) goto _output_error; }
    DISPLAYLEVEL(3, "OK \n");

     
    DISPLAYLEVEL(3, "test%3i : Content size verification : ", testNb++);
    {   ZSTD_CCtx* const cctx = ZSTD_createCCtx();
        size_t const srcSize = 5000;
        size_t const wrongSrcSize = (srcSize + 1000);
        ZSTD_parameters params = ZSTD_getParams(1, wrongSrcSize, 0);
        params.fParams.contentSizeFlag = 1;
        CHECK( ZSTD_compressBegin_advanced(cctx, NULL, 0, params, wrongSrcSize) );
        {   size_t const result = ZSTD_compressEnd(cctx, decodedBuffer, CNBuffSize, CNBuffer, srcSize);
            if (!ZSTD_isError(result)) goto _output_error;
            if (ZSTD_getErrorCode(result) != ZSTD_error_srcSize_wrong) goto _output_error;
            DISPLAYLEVEL(3, "OK : %s \n", ZSTD_getErrorName(result));
        }
        ZSTD_freeCCtx(cctx);
    }

     
    DISPLAYLEVEL(3, "test%3i : negative compression level : ", testNb++);
    {   ZSTD_CCtx* const cctx = ZSTD_createCCtx();
        size_t const srcSize = CNBuffSize / 5;
        int const compressionLevel = -1;

        assert(cctx != NULL);
        {   ZSTD_parameters const params = ZSTD_getParams(compressionLevel, srcSize, 0);
            size_t const cSize_1pass = ZSTD_compress_advanced(cctx,
                                        compressedBuffer, compressedBufferSize,
                                        CNBuffer, srcSize,
                                        NULL, 0,
                                        params);
            if (ZSTD_isError(cSize_1pass)) goto _output_error;

            CHECK( ZSTD_CCtx_setParameter(cctx, ZSTD_p_compressionLevel, (unsigned)compressionLevel) );
            {   ZSTD_inBuffer in = { CNBuffer, srcSize, 0 };
                ZSTD_outBuffer out = { compressedBuffer, compressedBufferSize, 0 };
                size_t const compressionResult = ZSTD_compress_generic(cctx, &out, &in, ZSTD_e_end);
                DISPLAYLEVEL(5, "simple=%zu vs %zu=advanced : ", cSize_1pass, out.pos);
                if (ZSTD_isError(compressionResult)) goto _output_error;
                if (out.pos != cSize_1pass) goto _output_error;
        }   }
        ZSTD_freeCCtx(cctx);
    }
    DISPLAYLEVEL(3, "OK \n");

     
    {   size_t const inputSize = CNBuffSize / 2;
        U64 xxh64;

        {   ZSTD_CCtx* const cctx = ZSTD_createCCtx();
            DISPLAYLEVEL(3, "test%3i : parameters in order : ", testNb++);
            assert(cctx != NULL);
            CHECK( ZSTD_CCtx_setParameter(cctx, ZSTD_p_compressionLevel, 2) );
            CHECK( ZSTD_CCtx_setParameter(cctx, ZSTD_p_enableLongDistanceMatching, 1) );
            CHECK( ZSTD_CCtx_setParameter(cctx, ZSTD_p_windowLog, 18) );
            {   ZSTD_inBuffer in = { CNBuffer, inputSize, 0 };
                ZSTD_outBuffer out = { compressedBuffer, ZSTD_compressBound(inputSize), 0 };
                size_t const result = ZSTD_compress_generic(cctx, &out, &in, ZSTD_e_end);
                if (result != 0) goto _output_error;
                if (in.pos != in.size) goto _output_error;
                cSize = out.pos;
                xxh64 = XXH64(out.dst, out.pos, 0);
            }
            DISPLAYLEVEL(3, "OK (compress : %u -> %u bytes)\n", (U32)inputSize, (U32)cSize);
            ZSTD_freeCCtx(cctx);
        }

        {   ZSTD_CCtx* cctx = ZSTD_createCCtx();
            DISPLAYLEVEL(3, "test%3i : parameters disordered : ", testNb++);
            CHECK( ZSTD_CCtx_setParameter(cctx, ZSTD_p_windowLog, 18) );
            CHECK( ZSTD_CCtx_setParameter(cctx, ZSTD_p_enableLongDistanceMatching, 1) );
            CHECK( ZSTD_CCtx_setParameter(cctx, ZSTD_p_compressionLevel, 2) );
            {   ZSTD_inBuffer in = { CNBuffer, inputSize, 0 };
                ZSTD_outBuffer out = { compressedBuffer, ZSTD_compressBound(inputSize), 0 };
                size_t const result = ZSTD_compress_generic(cctx, &out, &in, ZSTD_e_end);
                if (result != 0) goto _output_error;
                if (in.pos != in.size) goto _output_error;
                if (out.pos != cSize) goto _output_error;    
                if (XXH64(out.dst, out.pos, 0) != xxh64) goto _output_error;   
                DISPLAYLEVEL(3, "OK (compress : %u -> %u bytes)\n", (U32)inputSize, (U32)out.pos);
            }
            ZSTD_freeCCtx(cctx);
        }
    }

     
    {   ZSTD_CCtx* const cctx = ZSTD_createCCtx();
        size_t const inputSize = CNBuffSize / 2;    

         
        DISPLAYLEVEL(3, "test%3i : magic-less format test : ", testNb++);
        CHECK( ZSTD_CCtx_setParameter(cctx, ZSTD_p_format, ZSTD_f_zstd1_magicless) );
        {   ZSTD_inBuffer in = { CNBuffer, inputSize, 0 };
            ZSTD_outBuffer out = { compressedBuffer, ZSTD_compressBound(inputSize), 0 };
            size_t const result = ZSTD_compress_generic(cctx, &out, &in, ZSTD_e_end);
            if (result != 0) goto _output_error;
            if (in.pos != in.size) goto _output_error;
            cSize = out.pos;
        }
        DISPLAYLEVEL(3, "OK (compress : %u -> %u bytes)\n", (U32)inputSize, (U32)cSize);

        DISPLAYLEVEL(3, "test%3i : decompress normally (should fail) : ", testNb++);
        {   size_t const decodeResult = ZSTD_decompressDCtx(dctx, decodedBuffer, CNBuffSize, compressedBuffer, cSize);
            if (ZSTD_getErrorCode(decodeResult) != ZSTD_error_prefix_unknown) goto _output_error;
            DISPLAYLEVEL(3, "OK : %s \n", ZSTD_getErrorName(decodeResult));
        }

        DISPLAYLEVEL(3, "test%3i : decompress of magic-less frame : ", testNb++);
        ZSTD_DCtx_reset(dctx);
        CHECK( ZSTD_DCtx_setFormat(dctx, ZSTD_f_zstd1_magicless) );
        {   ZSTD_frameHeader zfh;
            size_t const zfhrt = ZSTD_getFrameHeader_advanced(&zfh, compressedBuffer, cSize, ZSTD_f_zstd1_magicless);
            if (zfhrt != 0) goto _output_error;
        }
        {   ZSTD_inBuffer in = { compressedBuffer, cSize, 0 };
            ZSTD_outBuffer out = { decodedBuffer, CNBuffSize, 0 };
            size_t const result = ZSTD_decompress_generic(dctx, &out, &in);
            if (result != 0) goto _output_error;
            if (in.pos != in.size) goto _output_error;
            if (out.pos != inputSize) goto _output_error;
            DISPLAYLEVEL(3, "OK : regenerated %u bytes \n", (U32)out.pos);
        }

        ZSTD_freeCCtx(cctx);
    }

     
    {   ZSTD_CCtx* const cctx = ZSTD_createCCtx();
        static const size_t dictSize = 65 KB;
        static const size_t blockSize = 100 KB;    
        size_t cSize2;

         
        DISPLAYLEVEL(3, "test%3i : Block compression test : ", testNb++);
        CHECK( ZSTD_compressBegin(cctx, 5) );
        CHECK( ZSTD_getBlockSize(cctx) >= blockSize);
        cSize = ZSTD_compressBlock(cctx, compressedBuffer, ZSTD_compressBound(blockSize), CNBuffer, blockSize);
        if (ZSTD_isError(cSize)) goto _output_error;
        DISPLAYLEVEL(3, "OK \n");

        DISPLAYLEVEL(3, "test%3i : Block decompression test : ", testNb++);
        CHECK( ZSTD_decompressBegin(dctx) );
        { CHECK_V(r, ZSTD_decompressBlock(dctx, decodedBuffer, CNBuffSize, compressedBuffer, cSize) );
          if (r != blockSize) goto _output_error; }
        DISPLAYLEVEL(3, "OK \n");

         
        DISPLAYLEVEL(3, "test%3i : Huge block streaming compression test : ", testNb++);
        CHECK( ZSTD_compressBegin(cctx, -99) );   
        CHECK( ZSTD_getBlockSize(cctx) >= blockSize);
        {   U64 const toCompress = 5000000000ULL;    
            U64 compressed = 0;
            while (compressed < toCompress) {
                size_t const blockCSize = ZSTD_compressBlock(cctx, compressedBuffer, ZSTD_compressBound(blockSize), CNBuffer, blockSize);
                if (ZSTD_isError(cSize)) goto _output_error;
                compressed += blockCSize;
            }
        }
        DISPLAYLEVEL(3, "OK \n");

         
        DISPLAYLEVEL(3, "test%3i : Dictionary Block compression test : ", testNb++);
        CHECK( ZSTD_compressBegin_usingDict(cctx, CNBuffer, dictSize, 5) );
        cSize = ZSTD_compressBlock(cctx, compressedBuffer, ZSTD_compressBound(blockSize), (char*)CNBuffer+dictSize, blockSize);
        if (ZSTD_isError(cSize)) goto _output_error;
        cSize2 = ZSTD_compressBlock(cctx, (char*)compressedBuffer+cSize, ZSTD_compressBound(blockSize), (char*)CNBuffer+dictSize+blockSize, blockSize);
        if (ZSTD_isError(cSize2)) goto _output_error;
        memcpy((char*)compressedBuffer+cSize, (char*)CNBuffer+dictSize+blockSize, blockSize);    
        cSize2 = ZSTD_compressBlock(cctx, (char*)compressedBuffer+cSize+blockSize, ZSTD_compressBound(blockSize),
                                          (char*)CNBuffer+dictSize+2*blockSize, blockSize);
        if (ZSTD_isError(cSize2)) goto _output_error;
        DISPLAYLEVEL(3, "OK \n");

        DISPLAYLEVEL(3, "test%3i : Dictionary Block decompression test : ", testNb++);
        CHECK( ZSTD_decompressBegin_usingDict(dctx, CNBuffer, dictSize) );
        { CHECK_V( r, ZSTD_decompressBlock(dctx, decodedBuffer, CNBuffSize, compressedBuffer, cSize) );
          if (r != blockSize) goto _output_error; }
        ZSTD_insertBlock(dctx, (char*)decodedBuffer+blockSize, blockSize);    
        { CHECK_V( r, ZSTD_decompressBlock(dctx, (char*)decodedBuffer+2*blockSize, CNBuffSize, (char*)compressedBuffer+cSize+blockSize, cSize2) );
          if (r != blockSize) goto _output_error; }
        DISPLAYLEVEL(3, "OK \n");

        DISPLAYLEVEL(3, "test%3i : Block compression with CDict : ", testNb++);
        {   ZSTD_CDict* const cdict = ZSTD_createCDict(CNBuffer, dictSize, 3);
            if (cdict==NULL) goto _output_error;
            CHECK( ZSTD_compressBegin_usingCDict(cctx, cdict) );
            CHECK( ZSTD_compressBlock(cctx, compressedBuffer, ZSTD_compressBound(blockSize), (char*)CNBuffer+dictSize, blockSize) );
            ZSTD_freeCDict(cdict);
        }
        DISPLAYLEVEL(3, "OK \n");

        ZSTD_freeCCtx(cctx);
    }
    ZSTD_freeDCtx(dctx);

     
    {   size_t sampleSize = 0;
        DISPLAYLEVEL(3, "test%3i : Long RLE test : ", testNb++);
        RDG_genBuffer(CNBuffer, sampleSize, compressibility, 0., seed+1);
        memset((char*)CNBuffer+sampleSize, 'B', 256 KB - 1);
        sampleSize += 256 KB - 1;
        RDG_genBuffer((char*)CNBuffer+sampleSize, 96 KB, compressibility, 0., seed+2);
        sampleSize += 96 KB;
        cSize = ZSTD_compress(compressedBuffer, ZSTD_compressBound(sampleSize), CNBuffer, sampleSize, 1);
        if (ZSTD_isError(cSize)) goto _output_error;
        { CHECK_V(regenSize, ZSTD_decompress(decodedBuffer, sampleSize, compressedBuffer, cSize));
          if (regenSize!=sampleSize) goto _output_error; }
        DISPLAYLEVEL(3, "OK \n");
    }

     
    #define ZEROESLENGTH 100
    DISPLAYLEVEL(3, "test%3i : compress %u zeroes : ", testNb++, ZEROESLENGTH);
    memset(CNBuffer, 0, ZEROESLENGTH);
    { CHECK_V(r, ZSTD_compress(compressedBuffer, ZSTD_compressBound(ZEROESLENGTH), CNBuffer, ZEROESLENGTH, 1) );
      cSize = r; }
    DISPLAYLEVEL(3, "OK (%u bytes : %.2f%%)\n", (U32)cSize, (double)cSize/ZEROESLENGTH*100);

    DISPLAYLEVEL(3, "test%3i : decompress %u zeroes : ", testNb++, ZEROESLENGTH);
    { CHECK_V(r, ZSTD_decompress(decodedBuffer, ZEROESLENGTH, compressedBuffer, cSize) );
      if (r != ZEROESLENGTH) goto _output_error; }
    DISPLAYLEVEL(3, "OK \n");

     
    #define _3BYTESTESTLENGTH 131000
    #define NB3BYTESSEQLOG   9
    #define NB3BYTESSEQ     (1 << NB3BYTESSEQLOG)
    #define NB3BYTESSEQMASK (NB3BYTESSEQ-1)
     
    {   BYTE _3BytesSeqs[NB3BYTESSEQ][3];
        U32 rSeed = 1;

         
        {   int i;
            for (i=0; i < NB3BYTESSEQ; i++) {
                _3BytesSeqs[i][0] = (BYTE)(FUZ_rand(&rSeed) & 255);
                _3BytesSeqs[i][1] = (BYTE)(FUZ_rand(&rSeed) & 255);
                _3BytesSeqs[i][2] = (BYTE)(FUZ_rand(&rSeed) & 255);
        }   }

         
        {   int i;
            for (i=0; i < _3BYTESTESTLENGTH; i += 3) {    
                U32 const id = FUZ_rand(&rSeed) & NB3BYTESSEQMASK;
                ((BYTE*)CNBuffer)[i+0] = _3BytesSeqs[id][0];
                ((BYTE*)CNBuffer)[i+1] = _3BytesSeqs[id][1];
                ((BYTE*)CNBuffer)[i+2] = _3BytesSeqs[id][2];
    }   }   }
    DISPLAYLEVEL(3, "test%3i : growing nbSeq : ", testNb++);
    {   ZSTD_CCtx* const cctx = ZSTD_createCCtx();
        size_t const maxNbSeq = _3BYTESTESTLENGTH / 3;
        size_t const bound = ZSTD_compressBound(_3BYTESTESTLENGTH);
        size_t nbSeq = 1;
        while (nbSeq <= maxNbSeq) {
          CHECK(ZSTD_compressCCtx(cctx, compressedBuffer, bound, CNBuffer, nbSeq * 3, 19));
           
          if (nbSeq < 100) {
            ++nbSeq;
          } else {
            nbSeq += (nbSeq >> 2);
          }
        }
        ZSTD_freeCCtx(cctx);
    }
    DISPLAYLEVEL(3, "OK \n");

    DISPLAYLEVEL(3, "test%3i : compress lots 3-bytes sequences : ", testNb++);
    { CHECK_V(r, ZSTD_compress(compressedBuffer, ZSTD_compressBound(_3BYTESTESTLENGTH),
                                 CNBuffer, _3BYTESTESTLENGTH, 19) );
      cSize = r; }
    DISPLAYLEVEL(3, "OK (%u bytes : %.2f%%)\n", (U32)cSize, (double)cSize/_3BYTESTESTLENGTH*100);

    DISPLAYLEVEL(3, "test%3i : decompress lots 3-bytes sequence : ", testNb++);
    { CHECK_V(r, ZSTD_decompress(decodedBuffer, _3BYTESTESTLENGTH, compressedBuffer, cSize) );
      if (r != _3BYTESTESTLENGTH) goto _output_error; }
    DISPLAYLEVEL(3, "OK \n");


    DISPLAYLEVEL(3, "test%3i : growing literals buffer : ", testNb++);
    RDG_genBuffer(CNBuffer, CNBuffSize, 0.0, 0.1, seed);
    {   ZSTD_CCtx* const cctx = ZSTD_createCCtx();
        size_t const bound = ZSTD_compressBound(CNBuffSize);
        size_t size = 1;
        while (size <= CNBuffSize) {
          CHECK(ZSTD_compressCCtx(cctx, compressedBuffer, bound, CNBuffer, size, 3));
           
          if (size < 100) {
            ++size;
          } else {
            size += (size >> 2);
          }
        }
        ZSTD_freeCCtx(cctx);
    }
    DISPLAYLEVEL(3, "OK \n");

    DISPLAYLEVEL(3, "test%3i : incompressible data and ill suited dictionary : ", testNb++);
    {    
        size_t dictSize = 16 KB;
        void* const dictBuffer = malloc(dictSize);
        size_t const totalSampleSize = 1 MB;
        size_t const sampleUnitSize = 8 KB;
        U32 const nbSamples = (U32)(totalSampleSize / sampleUnitSize);
        size_t* const samplesSizes = (size_t*) malloc(nbSamples * sizeof(size_t));
        if (!dictBuffer || !samplesSizes) goto _output_error;
        { U32 u; for (u=0; u<nbSamples; u++) samplesSizes[u] = sampleUnitSize; }
        dictSize = ZDICT_trainFromBuffer(dictBuffer, dictSize, CNBuffer, samplesSizes, nbSamples);
        if (ZDICT_isError(dictSize)) goto _output_error;
         
        {   U32 u;
            for (u = 0; u < CNBuffSize; ++u) {
              ((BYTE*)CNBuffer)[u] = 255 - ((BYTE*)CNBuffer)[u];
            }
        }
        {    
            size_t const inputSize = 500;
            size_t const outputSize = ZSTD_compressBound(inputSize);
            void* const outputBuffer = malloc(outputSize);
            ZSTD_CCtx* const cctx = ZSTD_createCCtx();
            if (!outputBuffer || !cctx) goto _output_error;
            CHECK(ZSTD_compress_usingDict(cctx, outputBuffer, outputSize, CNBuffer, inputSize, dictBuffer, dictSize, 1));
            free(outputBuffer);
            ZSTD_freeCCtx(cctx);
        }

        free(dictBuffer);
        free(samplesSizes);
    }
    DISPLAYLEVEL(3, "OK \n");


     
    DISPLAYLEVEL(3, "test%3i : frame compressed size of skippable frame : ", testNb++);
    {   const char* frame = "\x50\x2a\x4d\x18\x05\x0\x0\0abcde";
        size_t const frameSrcSize = 13;
        if (ZSTD_findFrameCompressedSize(frame, frameSrcSize) != frameSrcSize) goto _output_error; }
    DISPLAYLEVEL(3, "OK \n");

     
    DISPLAYLEVEL(3, "test%3i : testing ZSTD error code strings : ", testNb++);
    if (strcmp("No error detected", ZSTD_getErrorName((ZSTD_ErrorCode)(0-ZSTD_error_no_error))) != 0) goto _output_error;
    if (strcmp("No error detected", ZSTD_getErrorString(ZSTD_error_no_error)) != 0) goto _output_error;
    if (strcmp("Unspecified error code", ZSTD_getErrorString((ZSTD_ErrorCode)(0-ZSTD_error_GENERIC))) != 0) goto _output_error;
    if (strcmp("Error (generic)", ZSTD_getErrorName((size_t)0-ZSTD_error_GENERIC)) != 0) goto _output_error;
    if (strcmp("Error (generic)", ZSTD_getErrorString(ZSTD_error_GENERIC)) != 0) goto _output_error;
    if (strcmp("No error detected", ZSTD_getErrorName(ZSTD_error_GENERIC)) != 0) goto _output_error;
    DISPLAYLEVEL(3, "OK \n");

    DISPLAYLEVEL(3, "test%3i : testing ZSTD dictionary sizes : ", testNb++);
    RDG_genBuffer(CNBuffer, CNBuffSize, compressibility, 0., seed);
    {
        size_t const size = MIN(128 KB, CNBuffSize);
        ZSTD_CCtx* const cctx = ZSTD_createCCtx();
        ZSTD_CDict* const lgCDict = ZSTD_createCDict(CNBuffer, size, 1);
        ZSTD_CDict* const smCDict = ZSTD_createCDict(CNBuffer, 1 KB, 1);
        ZSTD_frameHeader lgHeader;
        ZSTD_frameHeader smHeader;

        CHECK_Z(ZSTD_compress_usingCDict(cctx, compressedBuffer, compressedBufferSize, CNBuffer, size, lgCDict));
        CHECK_Z(ZSTD_getFrameHeader(&lgHeader, compressedBuffer, compressedBufferSize));
        CHECK_Z(ZSTD_compress_usingCDict(cctx, compressedBuffer, compressedBufferSize, CNBuffer, size, smCDict));
        CHECK_Z(ZSTD_getFrameHeader(&smHeader, compressedBuffer, compressedBufferSize));

        if (lgHeader.windowSize != smHeader.windowSize) goto _output_error;

        ZSTD_freeCDict(smCDict);
        ZSTD_freeCDict(lgCDict);
        ZSTD_freeCCtx(cctx);
    }
    DISPLAYLEVEL(3, "OK \n");

    DISPLAYLEVEL(3, "test%3i : testing FSE_normalizeCount() PR#1255: ", testNb++);
    {
        short norm[32];
        unsigned count[32];
        unsigned const tableLog = 5;
        size_t const nbSeq = 32;
        unsigned const maxSymbolValue = 31;
        size_t i;

        for (i = 0; i < 32; ++i)
            count[i] = 1;
         
        FSE_normalizeCount(norm, tableLog, count, nbSeq, maxSymbolValue);
    }
    DISPLAYLEVEL(3, "OK \n");

_end:
    free(CNBuffer);
    free(compressedBuffer);
    free(decodedBuffer);
    return testResult;

_output_error:
    testResult = 1;
    DISPLAY("Error detected in Unit tests ! \n");
    goto _end;
}
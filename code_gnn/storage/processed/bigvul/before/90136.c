static int FUZ_mallocTests_internal(unsigned seed, double compressibility, unsigned part,
                void* inBuffer, size_t inSize, void* outBuffer, size_t outSize)
{
     
    if (g_displayLevel<3) return 0;

     
    if (!inBuffer || !outBuffer) {
        DISPLAY("Not enough memory, aborting\n");
        exit(1);
    }
    RDG_genBuffer(inBuffer, inSize, compressibility, 0.  , seed);

     
    if (part <= 1)
    {   int compressionLevel;
        for (compressionLevel=1; compressionLevel<=6; compressionLevel++) {
            mallocCounter_t malcount = INIT_MALLOC_COUNTER;
            ZSTD_customMem const cMem = { FUZ_mallocDebug, FUZ_freeDebug, &malcount };
            ZSTD_CCtx* const cctx = ZSTD_createCCtx_advanced(cMem);
            CHECK_Z( ZSTD_compressCCtx(cctx, outBuffer, outSize, inBuffer, inSize, compressionLevel) );
            ZSTD_freeCCtx(cctx);
            DISPLAYLEVEL(3, "compressCCtx level %i : ", compressionLevel);
            FUZ_displayMallocStats(malcount);
    }   }

     
    if (part <= 2)
    {   int compressionLevel;
        for (compressionLevel=1; compressionLevel<=6; compressionLevel++) {
            mallocCounter_t malcount = INIT_MALLOC_COUNTER;
            ZSTD_customMem const cMem = { FUZ_mallocDebug, FUZ_freeDebug, &malcount };
            ZSTD_CCtx* const cstream = ZSTD_createCStream_advanced(cMem);
            ZSTD_outBuffer out = { outBuffer, outSize, 0 };
            ZSTD_inBuffer in = { inBuffer, inSize, 0 };
            CHECK_Z( ZSTD_initCStream(cstream, compressionLevel) );
            CHECK_Z( ZSTD_compressStream(cstream, &out, &in) );
            CHECK_Z( ZSTD_endStream(cstream, &out) );
            ZSTD_freeCStream(cstream);
            DISPLAYLEVEL(3, "compressStream level %i : ", compressionLevel);
            FUZ_displayMallocStats(malcount);
    }   }

     
    if (part <= 3)
    {   U32 nbThreads;
        for (nbThreads=1; nbThreads<=4; nbThreads++) {
            int compressionLevel;
            for (compressionLevel=1; compressionLevel<=6; compressionLevel++) {
                mallocCounter_t malcount = INIT_MALLOC_COUNTER;
                ZSTD_customMem const cMem = { FUZ_mallocDebug, FUZ_freeDebug, &malcount };
                ZSTD_CCtx* const cctx = ZSTD_createCCtx_advanced(cMem);
                ZSTD_outBuffer out = { outBuffer, outSize, 0 };
                ZSTD_inBuffer in = { inBuffer, inSize, 0 };
                CHECK_Z( ZSTD_CCtx_setParameter(cctx, ZSTD_p_compressionLevel, (U32)compressionLevel) );
                CHECK_Z( ZSTD_CCtx_setParameter(cctx, ZSTD_p_nbWorkers, nbThreads) );
                while ( ZSTD_compress_generic(cctx, &out, &in, ZSTD_e_end) ) {}
                ZSTD_freeCCtx(cctx);
                DISPLAYLEVEL(3, "compress_generic,-T%u,end level %i : ",
                                nbThreads, compressionLevel);
                FUZ_displayMallocStats(malcount);
    }   }   }

     
    if (part <= 4)
    {   U32 nbThreads;
        for (nbThreads=1; nbThreads<=4; nbThreads++) {
            int compressionLevel;
            for (compressionLevel=1; compressionLevel<=6; compressionLevel++) {
                mallocCounter_t malcount = INIT_MALLOC_COUNTER;
                ZSTD_customMem const cMem = { FUZ_mallocDebug, FUZ_freeDebug, &malcount };
                ZSTD_CCtx* const cctx = ZSTD_createCCtx_advanced(cMem);
                ZSTD_outBuffer out = { outBuffer, outSize, 0 };
                ZSTD_inBuffer in = { inBuffer, inSize, 0 };
                CHECK_Z( ZSTD_CCtx_setParameter(cctx, ZSTD_p_compressionLevel, (U32)compressionLevel) );
                CHECK_Z( ZSTD_CCtx_setParameter(cctx, ZSTD_p_nbWorkers, nbThreads) );
                CHECK_Z( ZSTD_compress_generic(cctx, &out, &in, ZSTD_e_continue) );
                while ( ZSTD_compress_generic(cctx, &out, &in, ZSTD_e_end) ) {}
                ZSTD_freeCCtx(cctx);
                DISPLAYLEVEL(3, "compress_generic,-T%u,continue level %i : ",
                                nbThreads, compressionLevel);
                FUZ_displayMallocStats(malcount);
    }   }   }

    return 0;
}

static int FUZ_mallocTests(unsigned seed, double compressibility, unsigned part)
{
    size_t const inSize = 64 MB + 16 MB + 4 MB + 1 MB + 256 KB + 64 KB;  
    size_t const outSize = ZSTD_compressBound(inSize);
    void* const inBuffer = malloc(inSize);
    void* const outBuffer = malloc(outSize);
    int result;

     
    if (!inBuffer || !outBuffer) {
        DISPLAY("Not enough memory, aborting \n");
        exit(1);
    }

    result = FUZ_mallocTests_internal(seed, compressibility, part,
                    inBuffer, inSize, outBuffer, outSize);

    free(inBuffer);
    free(outBuffer);
    return result;
}

void FUZ_bug976(void)
{    
    assert(ZSTD_HASHLOG_MAX < 31);
    assert(ZSTD_CHAINLOG_MAX < 31);
}

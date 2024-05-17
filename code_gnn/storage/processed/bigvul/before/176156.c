void AMediaCodecCryptoInfo_setPattern(AMediaCodecCryptoInfo *info,
 cryptoinfo_pattern_t *pattern) {
    info->pattern.encryptBlocks = pattern->encryptBlocks;
    info->pattern.skipBlocks = pattern->skipBlocks;
}

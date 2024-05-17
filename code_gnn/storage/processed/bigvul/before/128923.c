    bool doReadKeyUsages(blink::WebCryptoKeyUsageMask& usages, bool& extractable)
    {
        COMPILE_ASSERT(blink::EndOfWebCryptoKeyUsage == (1 << 7) + 1, UpdateMe);
        const uint32_t allPossibleUsages = ExtractableUsage | EncryptUsage | DecryptUsage | SignUsage | VerifyUsage | DeriveKeyUsage | WrapKeyUsage | UnwrapKeyUsage | DeriveBitsUsage;

        uint32_t rawUsages;
        if (!doReadUint32(&rawUsages))
            return false;

        if (rawUsages & ~allPossibleUsages)
            return false;

        usages = 0;

        extractable = rawUsages & ExtractableUsage;

        if (rawUsages & EncryptUsage)
            usages |= blink::WebCryptoKeyUsageEncrypt;
        if (rawUsages & DecryptUsage)
            usages |= blink::WebCryptoKeyUsageDecrypt;
        if (rawUsages & SignUsage)
            usages |= blink::WebCryptoKeyUsageSign;
        if (rawUsages & VerifyUsage)
            usages |= blink::WebCryptoKeyUsageVerify;
        if (rawUsages & DeriveKeyUsage)
            usages |= blink::WebCryptoKeyUsageDeriveKey;
        if (rawUsages & WrapKeyUsage)
            usages |= blink::WebCryptoKeyUsageWrapKey;
        if (rawUsages & UnwrapKeyUsage)
            usages |= blink::WebCryptoKeyUsageUnwrapKey;
        if (rawUsages & DeriveBitsUsage)
            usages |= blink::WebCryptoKeyUsageDeriveBits;

        return true;
    }

    void doWriteKeyUsages(const blink::WebCryptoKeyUsageMask usages, bool extractable)
    {
        COMPILE_ASSERT(blink::EndOfWebCryptoKeyUsage == (1 << 7) + 1, UpdateMe);

        uint32_t value = 0;

        if (extractable)
            value |= ExtractableUsage;

        if (usages & blink::WebCryptoKeyUsageEncrypt)
            value |= EncryptUsage;
        if (usages & blink::WebCryptoKeyUsageDecrypt)
            value |= DecryptUsage;
        if (usages & blink::WebCryptoKeyUsageSign)
            value |= SignUsage;
        if (usages & blink::WebCryptoKeyUsageVerify)
            value |= VerifyUsage;
        if (usages & blink::WebCryptoKeyUsageDeriveKey)
            value |= DeriveKeyUsage;
        if (usages & blink::WebCryptoKeyUsageWrapKey)
            value |= WrapKeyUsage;
        if (usages & blink::WebCryptoKeyUsageUnwrapKey)
            value |= UnwrapKeyUsage;
        if (usages & blink::WebCryptoKeyUsageDeriveBits)
            value |= DeriveBitsUsage;

        doWriteUint32(value);
    }

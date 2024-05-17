    void doWriteAesKey(const blink::WebCryptoKey& key)
    {
        ASSERT(key.algorithm().paramsType() == blink::WebCryptoKeyAlgorithmParamsTypeAes);

        append(static_cast<uint8_t>(AesKeyTag));
        doWriteAlgorithmId(key.algorithm().id());
        ASSERT(!(key.algorithm().aesParams()->lengthBits() % 8));
        doWriteUint32(key.algorithm().aesParams()->lengthBits() / 8);
    }

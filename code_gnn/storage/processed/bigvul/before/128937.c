    void doWriteRsaHashedKey(const blink::WebCryptoKey& key)
    {
        ASSERT(key.algorithm().rsaHashedParams());
        append(static_cast<uint8_t>(RsaHashedKeyTag));

        doWriteAlgorithmId(key.algorithm().id());

        switch (key.type()) {
        case blink::WebCryptoKeyTypePublic:
            doWriteUint32(PublicKeyType);
            break;
        case blink::WebCryptoKeyTypePrivate:
            doWriteUint32(PrivateKeyType);
            break;
        case blink::WebCryptoKeyTypeSecret:
            ASSERT_NOT_REACHED();
        }

        const blink::WebCryptoRsaHashedKeyAlgorithmParams* params = key.algorithm().rsaHashedParams();
        doWriteUint32(params->modulusLengthBits());
        doWriteUint32(params->publicExponent().size());
        append(params->publicExponent().data(), params->publicExponent().size());
        doWriteAlgorithmId(key.algorithm().rsaHashedParams()->hash().id());
    }

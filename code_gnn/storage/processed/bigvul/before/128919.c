    bool doReadAesKey(blink::WebCryptoKeyAlgorithm& algorithm, blink::WebCryptoKeyType& type)
    {
        blink::WebCryptoAlgorithmId id;
        if (!doReadAlgorithmId(id))
            return false;
        uint32_t lengthBytes;
        if (!doReadUint32(&lengthBytes))
            return false;
        algorithm = blink::WebCryptoKeyAlgorithm::createAes(id, lengthBytes * 8);
        type = blink::WebCryptoKeyTypeSecret;
        return !algorithm.isNull();
    }

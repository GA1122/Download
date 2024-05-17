    bool doReadHmacKey(blink::WebCryptoKeyAlgorithm& algorithm, blink::WebCryptoKeyType& type)
    {
        uint32_t lengthBytes;
        if (!doReadUint32(&lengthBytes))
            return false;
        blink::WebCryptoAlgorithmId hash;
        if (!doReadAlgorithmId(hash))
            return false;
        algorithm = blink::WebCryptoKeyAlgorithm::createHmac(hash, lengthBytes * 8);
        type = blink::WebCryptoKeyTypeSecret;
        return !algorithm.isNull();
    }

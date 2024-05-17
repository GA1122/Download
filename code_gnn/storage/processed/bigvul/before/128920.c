    bool doReadAlgorithmId(blink::WebCryptoAlgorithmId& id)
    {
        uint32_t rawId;
        if (!doReadUint32(&rawId))
            return false;

        switch (static_cast<CryptoKeyAlgorithmTag>(rawId)) {
        case AesCbcTag:
            id = blink::WebCryptoAlgorithmIdAesCbc;
            return true;
        case HmacTag:
            id = blink::WebCryptoAlgorithmIdHmac;
            return true;
        case RsaSsaPkcs1v1_5Tag:
            id = blink::WebCryptoAlgorithmIdRsaSsaPkcs1v1_5;
            return true;
        case Sha1Tag:
            id = blink::WebCryptoAlgorithmIdSha1;
            return true;
        case Sha256Tag:
            id = blink::WebCryptoAlgorithmIdSha256;
            return true;
        case Sha384Tag:
            id = blink::WebCryptoAlgorithmIdSha384;
            return true;
        case Sha512Tag:
            id = blink::WebCryptoAlgorithmIdSha512;
            return true;
        case AesGcmTag:
            id = blink::WebCryptoAlgorithmIdAesGcm;
            return true;
        case RsaOaepTag:
            id = blink::WebCryptoAlgorithmIdRsaOaep;
            return true;
        case AesCtrTag:
            id = blink::WebCryptoAlgorithmIdAesCtr;
            return true;
        case AesKwTag:
            id = blink::WebCryptoAlgorithmIdAesKw;
            return true;
        }

        return false;
    }

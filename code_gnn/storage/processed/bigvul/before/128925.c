    bool doReadRsaHashedKey(blink::WebCryptoKeyAlgorithm& algorithm, blink::WebCryptoKeyType& type)
    {
        blink::WebCryptoAlgorithmId id;
        if (!doReadAlgorithmId(id))
            return false;

        uint32_t rawType;
        if (!doReadUint32(&rawType))
            return false;

        switch (static_cast<AssymetricCryptoKeyType>(rawType)) {
        case PublicKeyType:
            type = blink::WebCryptoKeyTypePublic;
            break;
        case PrivateKeyType:
            type = blink::WebCryptoKeyTypePrivate;
            break;
        default:
            return false;
        }

        uint32_t modulusLengthBits;
        if (!doReadUint32(&modulusLengthBits))
            return false;

        uint32_t publicExponentSize;
        if (!doReadUint32(&publicExponentSize))
            return false;

        if (m_position + publicExponentSize > m_length)
            return false;

        const uint8_t* publicExponent = m_buffer + m_position;
        m_position += publicExponentSize;

        blink::WebCryptoAlgorithmId hash;
        if (!doReadAlgorithmId(hash))
            return false;
        algorithm = blink::WebCryptoKeyAlgorithm::createRsaHashed(id, modulusLengthBits, publicExponent, publicExponentSize, hash);

        return !algorithm.isNull();
    }

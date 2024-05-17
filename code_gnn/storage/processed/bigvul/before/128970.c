    bool readCryptoKey(v8::Handle<v8::Value>* value)
    {
        uint32_t rawKeyType;
        if (!doReadUint32(&rawKeyType))
            return false;

        blink::WebCryptoKeyAlgorithm algorithm;
        blink::WebCryptoKeyType type = blink::WebCryptoKeyTypeSecret;

        switch (static_cast<CryptoKeySubTag>(rawKeyType)) {
        case AesKeyTag:
            if (!doReadAesKey(algorithm, type))
                return false;
            break;
        case HmacKeyTag:
            if (!doReadHmacKey(algorithm, type))
                return false;
            break;
        case RsaHashedKeyTag:
            if (!doReadRsaHashedKey(algorithm, type))
                return false;
            break;
        default:
            return false;
        }

        blink::WebCryptoKeyUsageMask usages;
        bool extractable;
        if (!doReadKeyUsages(usages, extractable))
            return false;

        uint32_t keyDataLength;
        if (!doReadUint32(&keyDataLength))
            return false;

        if (m_position + keyDataLength > m_length)
            return false;

        const uint8_t* keyData = m_buffer + m_position;
        m_position += keyDataLength;

        blink::WebCryptoKey key = blink::WebCryptoKey::createNull();
        if (!blink::Platform::current()->crypto()->deserializeKeyForClone(
            algorithm, type, extractable, usages, keyData, keyDataLength, key)) {
            return false;
        }

        *value = toV8(CryptoKey::create(key), m_scriptState->context()->Global(), isolate());
        return true;
    }

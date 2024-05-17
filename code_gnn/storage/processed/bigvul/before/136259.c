bool CSPSourceList::parseHash(const UChar* begin, const UChar* end, DigestValue& hash, ContentSecurityPolicyHashAlgorithm& hashAlgorithm)
{
    static const struct {
        const char* prefix;
        ContentSecurityPolicyHashAlgorithm type;
    } kSupportedPrefixes[] = {
        { "'sha1-", ContentSecurityPolicyHashAlgorithmSha1 },
        { "'sha256-", ContentSecurityPolicyHashAlgorithmSha256 },
        { "'sha384-", ContentSecurityPolicyHashAlgorithmSha384 },
        { "'sha512-", ContentSecurityPolicyHashAlgorithmSha512 },
        { "'sha-256-", ContentSecurityPolicyHashAlgorithmSha256 },
        { "'sha-384-", ContentSecurityPolicyHashAlgorithmSha384 },
        { "'sha-512-", ContentSecurityPolicyHashAlgorithmSha512 }
    };

    String prefix;
    hashAlgorithm = ContentSecurityPolicyHashAlgorithmNone;
    size_t hashLength = end - begin;

    for (const auto& algorithm : kSupportedPrefixes) {
        if (hashLength > strlen(algorithm.prefix) && equalIgnoringCase(algorithm.prefix, begin, strlen(algorithm.prefix))) {
            prefix = algorithm.prefix;
            hashAlgorithm = algorithm.type;
            break;
        }
    }

    if (hashAlgorithm == ContentSecurityPolicyHashAlgorithmNone)
        return true;

    const UChar* position = begin + prefix.length();
    const UChar* hashBegin = position;

    ASSERT(position < end);
    skipWhile<UChar, isBase64EncodedCharacter>(position, end);
    ASSERT(hashBegin <= position);

    if (position < end)
        skipExactly<UChar>(position, position + 1, '=');
    if (position < end)
        skipExactly<UChar>(position, position + 1, '=');

    if (position + 1 != end || *position != '\'' || position == hashBegin)
        return false;

    Vector<char> hashVector;
    base64Decode(normalizeToBase64(String(hashBegin, position - hashBegin)), hashVector);
    if (hashVector.size() > kMaxDigestSize)
        return false;
    hash.append(reinterpret_cast<uint8_t*>(hashVector.data()), hashVector.size());
    return true;
}

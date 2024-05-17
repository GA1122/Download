void CSPSourceList::addSourceHash(const ContentSecurityPolicyHashAlgorithm& algorithm, const DigestValue& hash)
{
    m_hashes.add(CSPHashValue(algorithm, hash));
     m_hashAlgorithmsUsed |= algorithm;
 }

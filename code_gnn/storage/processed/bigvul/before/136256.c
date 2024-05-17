bool CSPSourceList::isHashOrNoncePresent() const
{
    return !m_nonces.isEmpty() || m_hashAlgorithmsUsed != ContentSecurityPolicyHashAlgorithmNone;
}

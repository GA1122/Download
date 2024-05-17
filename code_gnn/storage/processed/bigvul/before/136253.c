bool CSPSourceList::allowHash(const CSPHashValue& hashValue) const
{
    return m_hashes.contains(hashValue);
}

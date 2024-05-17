CSPSourceList::CSPSourceList(ContentSecurityPolicy* policy, const String& directiveName)
    : m_policy(policy)
    , m_directiveName(directiveName)
    , m_allowSelf(false)
    , m_allowStar(false)
    , m_allowInline(false)
    , m_allowEval(false)
    , m_hashAlgorithmsUsed(0)
{
}

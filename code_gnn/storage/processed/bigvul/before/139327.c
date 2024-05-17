void HarfBuzzShaper::setDrawRange(int from, int to)
{
    ASSERT_WITH_SECURITY_IMPLICATION(from >= 0);
    ASSERT_WITH_SECURITY_IMPLICATION(to <= m_run.length());
    m_fromIndex = from;
    m_toIndex = to;
}

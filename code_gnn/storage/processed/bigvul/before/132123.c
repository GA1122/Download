void LayoutBlockFlow::setDidBreakAtLineToAvoidWidow()
{
    ASSERT(!shouldBreakAtLineToAvoidWidow());

    ASSERT(m_rareData);

    m_rareData->m_didBreakAtLineToAvoidWidow = true;
}

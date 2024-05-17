void RenderBox::deleteLineBoxWrapper()
{
    if (inlineBoxWrapper()) {
        if (!documentBeingDestroyed())
            inlineBoxWrapper()->remove();
        inlineBoxWrapper()->destroy();
        ASSERT(m_rareData);
        m_rareData->m_inlineBoxWrapper = 0;
    }
}

void RenderBox::dirtyLineBoxes(bool fullLayout)
{
    if (inlineBoxWrapper()) {
        if (fullLayout) {
            inlineBoxWrapper()->destroy();
            ASSERT(m_rareData);
            m_rareData->m_inlineBoxWrapper = 0;
        } else {
            inlineBoxWrapper()->dirtyLineBoxes();
        }
    }
}

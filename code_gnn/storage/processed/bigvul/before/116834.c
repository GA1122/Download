int PopupContainer::layoutAndGetRTLOffset()
{
    m_listBox->layout();

    m_listBox->move(kBorderSize, kBorderSize);

    int listBoxWidth = m_listBox->width() + kBorderSize * 2;
    resize(listBoxWidth, m_listBox->height() + kBorderSize * 2);
    invalidate();

    return m_originalFrameRect.width() - listBoxWidth;
}

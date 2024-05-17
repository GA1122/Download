PassRefPtr<ClientRectList> Range::getClientRects() const
{
    if (!m_start.container())
        return ClientRectList::create();

    m_ownerDocument->updateLayoutIgnorePendingStylesheets();

    Vector<FloatQuad> quads;
    getBorderAndTextQuads(quads);

    return ClientRectList::create(quads);
}

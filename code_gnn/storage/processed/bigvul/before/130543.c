 bool DisplayItemList::lastDisplayItemIsNoopBegin() const
 {
     if (m_newDisplayItems.isEmpty())
        return false;

    const auto& lastDisplayItem = m_newDisplayItems.last();
    return lastDisplayItem.isBegin() && !lastDisplayItem.drawsContent();
}

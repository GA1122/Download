void LayoutBlockFlow::moveAllChildrenIncludingFloatsTo(LayoutBlock* toBlock, bool fullRemoveInsert)
{
    LayoutBlockFlow* toBlockFlow = toLayoutBlockFlow(toBlock);
    moveAllChildrenTo(toBlockFlow, fullRemoveInsert);

    if (m_floatingObjects) {
        if (!toBlockFlow->m_floatingObjects)
            toBlockFlow->createFloatingObjects();

        const FloatingObjectSet& fromFloatingObjectSet = m_floatingObjects->set();
        FloatingObjectSetIterator end = fromFloatingObjectSet.end();

        for (FloatingObjectSetIterator it = fromFloatingObjectSet.begin(); it != end; ++it) {
            const FloatingObject& floatingObject = *it->get();

            if (toBlockFlow->containsFloat(floatingObject.layoutObject()))
                continue;

            toBlockFlow->m_floatingObjects->add(floatingObject.unsafeClone());
        }
    }

}

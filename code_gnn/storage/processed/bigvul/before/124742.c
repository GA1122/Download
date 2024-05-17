void RenderBlockFlow::moveAllChildrenIncludingFloatsTo(RenderBlock* toBlock, bool fullRemoveInsert)
{
    RenderBlockFlow* toBlockFlow = toRenderBlockFlow(toBlock);
    moveAllChildrenTo(toBlockFlow, fullRemoveInsert);

    if (m_floatingObjects) {
        if (!toBlockFlow->m_floatingObjects)
            toBlockFlow->createFloatingObjects();

        const FloatingObjectSet& fromFloatingObjectSet = m_floatingObjects->set();
        FloatingObjectSetIterator end = fromFloatingObjectSet.end();

        for (FloatingObjectSetIterator it = fromFloatingObjectSet.begin(); it != end; ++it) {
            FloatingObject* floatingObject = *it;

            if (toBlockFlow->containsFloat(floatingObject->renderer()))
                continue;

            toBlockFlow->m_floatingObjects->add(floatingObject->unsafeClone());
        }
    }

}

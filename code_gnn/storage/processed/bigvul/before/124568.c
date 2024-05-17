void RenderBlock::insertPositionedObject(RenderBox* o)
{
    ASSERT(!isAnonymousBlock());

    if (o->isRenderFlowThread())
        return;

    insertIntoTrackedRendererMaps(o, gPositionedDescendantsMap, gPositionedContainerMap);
}

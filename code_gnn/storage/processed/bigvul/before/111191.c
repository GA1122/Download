static bool isFixedPositionedContainer(RenderLayer* layer)
{
    RenderObject* o = layer->renderer();
    return o->isRenderView() || (o->isOutOfFlowPositioned() && o->style()->position() == FixedPosition);
}

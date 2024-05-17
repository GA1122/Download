static bool isPositionedContainer(RenderLayer* layer)
{
    RenderObject* o = layer->renderer();
    return o->isRenderView() || o->isOutOfFlowPositioned() || o->isRelPositioned() || layer->hasTransform();
}

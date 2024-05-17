RenderInline* RenderBlock::inlineElementContinuation() const
{
    RenderBoxModelObject* continuation = this->continuation();
    return continuation && continuation->isInline() ? toRenderInline(continuation) : 0;
}

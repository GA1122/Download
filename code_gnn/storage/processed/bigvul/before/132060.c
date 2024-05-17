RootInlineBox* LayoutBlockFlow::createRootInlineBox()
{
    return new RootInlineBox(*this);
}

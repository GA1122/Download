LayoutSize RenderBox::topLeftLocationOffset() const
{
    RenderBlock* containerBlock = containingBlock();
    if (!containerBlock || containerBlock == this)
        return locationOffset();

    LayoutRect rect(frameRect());
    containerBlock->flipForWritingMode(rect);  
    return LayoutSize(rect.x(), rect.y());
}

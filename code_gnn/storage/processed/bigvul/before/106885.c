IntSize RenderBox::locationOffsetIncludingFlipping() const
{
    RenderBlock* containerBlock = containingBlock();
    if (!containerBlock || containerBlock == this)
        return locationOffset();
    
    IntRect rect(frameRect());
    containerBlock->flipForWritingMode(rect);  
    return IntSize(rect.x(), rect.y());
}

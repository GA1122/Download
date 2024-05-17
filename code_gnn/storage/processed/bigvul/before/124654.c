void RenderBlock::updateDragState(bool dragOn)
{
    RenderBox::updateDragState(dragOn);
    if (continuation())
        continuation()->updateDragState(dragOn);
}

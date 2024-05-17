void RenderBox::updateGridPositionAfterStyleChange(const RenderStyle* oldStyle)
{
    if (!oldStyle || !parent() || !parent()->isRenderGrid())
        return;

    if (oldStyle->gridColumnStart() == style()->gridColumnStart()
        && oldStyle->gridColumnEnd() == style()->gridColumnEnd()
        && oldStyle->gridRowStart() == style()->gridRowStart()
        && oldStyle->gridRowEnd() == style()->gridRowEnd()
        && oldStyle->order() == style()->order()
        && oldStyle->hasOutOfFlowPosition() == style()->hasOutOfFlowPosition())
        return;

    toRenderGrid(parent())->dirtyGrid();
}

LayoutRect RenderBox::clipRect(const LayoutPoint& location)
{
    LayoutRect borderBoxRect = this->borderBoxRect();
    LayoutRect clipRect = LayoutRect(borderBoxRect.location() + location, borderBoxRect.size());

    if (!style()->clipLeft().isAuto()) {
        LayoutUnit c = valueForLength(style()->clipLeft(), borderBoxRect.width());
        clipRect.move(c, 0);
        clipRect.contract(c, 0);
    }

    if (!style()->clipRight().isAuto())
        clipRect.contract(width() - valueForLength(style()->clipRight(), width()), 0);

    if (!style()->clipTop().isAuto()) {
        LayoutUnit c = valueForLength(style()->clipTop(), borderBoxRect.height());
        clipRect.move(0, c);
        clipRect.contract(0, c);
    }

    if (!style()->clipBottom().isAuto())
        clipRect.contract(0, height() - valueForLength(style()->clipBottom(), height()));

    return clipRect;
}

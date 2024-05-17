void RenderBox::positionLineBox(InlineBox* box)
{
    if (isPositioned()) {
        bool wasInline = style()->isOriginalDisplayInlineType();
        if (wasInline) {
            layer()->setStaticInlinePosition(lroundf(box->logicalLeft()));
            if (style()->hasStaticInlinePosition(box->isHorizontal()))
                setChildNeedsLayout(true, false);  
        } else {
            layer()->setStaticBlockPosition(box->logicalTop());
            if (style()->hasStaticBlockPosition(box->isHorizontal()))
                setChildNeedsLayout(true, false);  
        }

        box->remove();
        box->destroy(renderArena());
    } else if (isReplaced()) {
        setLocation(roundedIntPoint(FloatPoint(box->x(), box->y())));
        m_inlineBoxWrapper = box;
    }
}

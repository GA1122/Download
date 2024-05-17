void RenderFlexibleBox::resetAutoMarginsAndLogicalTopInCrossAxis(RenderBox* child)
{
    if (hasAutoMarginsInCrossAxis(child)) {
        child->updateLogicalHeight();
        if (isHorizontalFlow()) {
            if (child->style()->marginTop().isAuto())
                child->setMarginTop(0);
            if (child->style()->marginBottom().isAuto())
                child->setMarginBottom(0);
        } else {
            if (child->style()->marginLeft().isAuto())
                child->setMarginLeft(0);
            if (child->style()->marginRight().isAuto())
                child->setMarginRight(0);
        }
    }
}

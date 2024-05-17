void LayoutBlockFlow::positionDialog()
{
    HTMLDialogElement* dialog = toHTMLDialogElement(node());
    if (dialog->centeringMode() == HTMLDialogElement::NotCentered)
        return;

    bool canCenterDialog = (style()->position() == AbsolutePosition || style()->position() == FixedPosition)
        && style()->hasAutoTopAndBottom();

    if (dialog->centeringMode() == HTMLDialogElement::Centered) {
        if (canCenterDialog)
            setY(dialog->centeredPosition());
        return;
    }

    ASSERT(dialog->centeringMode() == HTMLDialogElement::NeedsCentering);
    if (!canCenterDialog) {
        dialog->setNotCentered();
        return;
    }

    FrameView* frameView = document().view();
    LayoutUnit top = (style()->position() == FixedPosition) ? 0 : frameView->scrollOffset().height();
    int visibleHeight = frameView->visibleContentRect(IncludeScrollbars).height();
    if (size().height() < visibleHeight)
        top += (visibleHeight - size().height()) / 2;
    setY(top);
    dialog->setCentered(top);
}

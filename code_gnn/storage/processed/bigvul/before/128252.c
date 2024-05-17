void FrameView::frameRectsChanged()
{
    if (layoutSizeFixedToFrameSize())
        setLayoutSizeInternal(frameRect().size());

    ScrollView::frameRectsChanged();
}

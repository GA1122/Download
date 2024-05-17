bool RenderBox::canAutoscroll() const
{
    if (node() && node()->isDocumentNode())
        return view()->frameView()->isScrollable();

    return canBeScrolledAndHasScrollableArea();
}

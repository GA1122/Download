static bool needsLayoutRecursive(FrameView* view)
{
    if (view->needsLayout())
        return true;

    bool subframesNeedsLayout = false;
    const HashSet<RefPtr<Widget> >* viewChildren = view->children();
    HashSet<RefPtr<Widget> >::const_iterator end = viewChildren->end();
    for (HashSet<RefPtr<Widget> >::const_iterator current = viewChildren->begin(); current != end && !subframesNeedsLayout; ++current) {
        Widget* widget = (*current).get();
        if (widget->isFrameView())
            subframesNeedsLayout |= needsLayoutRecursive(static_cast<FrameView*>(widget));
    }

    return subframesNeedsLayout;
}

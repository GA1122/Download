void FrameView::scrollbarExistenceDidChange()
{
    if (!frame().view())
        return;

    bool useOverlayScrollbars = ScrollbarTheme::theme()->usesOverlayScrollbars();

    if (!useOverlayScrollbars && needsLayout())
        layout();

    if (renderView() && renderView()->usesCompositing()) {
        renderView()->compositor()->frameViewScrollbarsExistenceDidChange();

        if (!useOverlayScrollbars)
            renderView()->compositor()->frameViewDidChangeSize();
    }
}

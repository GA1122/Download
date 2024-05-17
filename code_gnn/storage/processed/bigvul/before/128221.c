void FrameView::applyOverflowToViewportAndSetRenderer(RenderObject* o, ScrollbarMode& hMode, ScrollbarMode& vMode)
{

    EOverflow overflowX = o->style()->overflowX();
    EOverflow overflowY = o->style()->overflowY();

    if (o->isSVGRoot()) {
        if (toRenderSVGRoot(o)->isEmbeddedThroughSVGImage())
            return;

        if (toRenderSVGRoot(o)->isEmbeddedThroughFrameContainingSVGDocument()) {
            overflowX = OHIDDEN;
            overflowY = OHIDDEN;
        }
    }

    bool ignoreOverflowHidden = false;
    if (m_frame->settings()->ignoreMainFrameOverflowHiddenQuirk() && m_frame->isMainFrame())
        ignoreOverflowHidden = true;

    switch (overflowX) {
        case OHIDDEN:
            if (!ignoreOverflowHidden)
                hMode = ScrollbarAlwaysOff;
            break;
        case OSCROLL:
            hMode = ScrollbarAlwaysOn;
            break;
        case OAUTO:
            hMode = ScrollbarAuto;
            break;
        default:
            ;
    }

     switch (overflowY) {
        case OHIDDEN:
            if (!ignoreOverflowHidden)
                vMode = ScrollbarAlwaysOff;
            break;
        case OSCROLL:
            vMode = ScrollbarAlwaysOn;
            break;
        case OAUTO:
            vMode = ScrollbarAuto;
            break;
        default:
            ;
    }

    m_viewportRenderer = o;
}

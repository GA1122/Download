void Document::recalcStyle(StyleChange change)
{
    ASSERT(!view() || !view()->isPainting());
    if (view() && view()->isPainting())
        return;
    
    if (m_inStyleRecalc)
        return;  

#if PLATFORM(CHROMIUM)
    TRACE_EVENT0("webkit", "Document::recalcStyle");
#endif


    if (m_styleSheetCollection->needsUpdateActiveStylesheetsOnStyleRecalc())
        m_styleSheetCollection->updateActiveStyleSheets(DocumentStyleSheetCollection::FullUpdate);

    InspectorInstrumentationCookie cookie = InspectorInstrumentation::willRecalculateStyle(this);

    if (m_elemSheet && m_elemSheet->contents()->usesRemUnits())
        m_styleSheetCollection->setUsesRemUnit(true);

    m_inStyleRecalc = true;
    {
        PostAttachCallbackDisabler disabler(this);
        WidgetHierarchyUpdatesSuspensionScope suspendWidgetHierarchyUpdates;

        RefPtr<FrameView> frameView = view();
        if (frameView) {
            frameView->pauseScheduledEvents();
            frameView->beginDeferredRepaints();
        }

        ASSERT(!renderer() || renderArena());
        if (!renderer() || !renderArena())
            goto bailOut;

        if (m_pendingStyleRecalcShouldForce)
            change = Force;

        if ((change == Force) || (shouldDisplaySeamlesslyWithParent() && (change >= Inherit))) {
            m_hasNodesWithPlaceholderStyle = false;
            
            RefPtr<RenderStyle> documentStyle = StyleResolver::styleForDocument(this, m_styleResolver ? m_styleResolver->fontSelector() : 0);
            StyleChange ch = Node::diff(documentStyle.get(), renderer()->style(), this);
            if (ch != NoChange)
                renderer()->setStyle(documentStyle.release());
        }

        for (Node* n = firstChild(); n; n = n->nextSibling()) {
            if (!n->isElementNode())
                continue;
            Element* element = toElement(n);
            if (change >= Inherit || element->childNeedsStyleRecalc() || element->needsStyleRecalc())
                element->recalcStyle(change);
        }

#if USE(ACCELERATED_COMPOSITING)
        if (view())
            view()->updateCompositingLayersAfterStyleChange();
#endif

    bailOut:
        clearNeedsStyleRecalc();
        clearChildNeedsStyleRecalc();
        unscheduleStyleRecalc();

        m_inStyleRecalc = false;

        if (m_styleResolver)
            m_styleSheetCollection->resetCSSFeatureFlags();

        if (frameView) {
            frameView->resumeScheduledEvents();
            frameView->endDeferredRepaints();
        }
    }

    if (m_closeAfterStyleRecalc) {
        m_closeAfterStyleRecalc = false;
        implicitClose();
    }

    InspectorInstrumentation::didRecalculateStyle(cookie);
}

PassRefPtr<RenderStyle> StyleResolver::styleForElement(Element* element, RenderStyle* defaultParent, StyleSharingBehavior sharingBehavior,
    RuleMatchingBehavior matchingBehavior, RenderRegion* regionForStyling)
{
    ASSERT(document().frame());
    ASSERT(documentSettings());

    if (sharingBehavior == AllowStyleSharing && !element->document().haveStylesheetsLoaded() && !element->renderer()) {
        if (!s_styleNotYetAvailable) {
            s_styleNotYetAvailable = RenderStyle::create().leakRef();
            s_styleNotYetAvailable->setDisplay(NONE);
            s_styleNotYetAvailable->font().update(m_fontSelector);
        }
        element->document().setHasNodesWithPlaceholderStyle();
        return s_styleNotYetAvailable;
    }

    didAccess();

    if (element == document().documentElement())
        resetDirectionAndWritingModeOnDocument(document());
    StyleResolverState state(document(), element, defaultParent, regionForStyling);

    if (sharingBehavior == AllowStyleSharing && !state.distributedToInsertionPoint() && state.parentStyle()) {
        SharedStyleFinder styleFinder(state.elementContext(), m_features, m_siblingRuleSet.get(), m_uncommonAttributeRuleSet.get(), *this);
        if (RefPtr<RenderStyle> sharedStyle = styleFinder.findSharedStyle())
            return sharedStyle.release();
    }

    if (state.parentStyle()) {
        state.setStyle(RenderStyle::create());
        state.style()->inheritFrom(state.parentStyle(), isAtShadowBoundary(element) ? RenderStyle::AtShadowBoundary : RenderStyle::NotAtShadowBoundary);
    } else {
        state.setStyle(defaultStyleForElement());
        state.setParentStyle(RenderStyle::clone(state.style()));
    }
    if (state.distributedToInsertionPoint()) {
        if (Element* parent = element->parentElement()) {
            if (RenderStyle* styleOfShadowHost = parent->renderStyle())
                state.style()->setUserModify(styleOfShadowHost->userModify());
        }
    }

    state.fontBuilder().initForStyleResolve(state.document(), state.style(), state.useSVGZoomRules());

    if (element->isLink()) {
        state.style()->setIsLink(true);
        EInsideLink linkState = state.elementLinkState();
        if (linkState != NotInsideLink) {
            bool forceVisited = InspectorInstrumentation::forcePseudoState(element, CSSSelector::PseudoVisited);
            if (forceVisited)
                linkState = InsideVisitedLink;
        }
        state.style()->setInsideLink(linkState);
    }

    bool needsCollection = false;
    CSSDefaultStyleSheets::ensureDefaultStyleSheetsForElement(element, needsCollection);
    if (needsCollection) {
        collectFeatures();
        m_inspectorCSSOMWrappers.reset();
    }

    {
        ElementRuleCollector collector(state.elementContext(), m_selectorFilter, state.style());
        collector.setRegionForStyling(regionForStyling);

        if (matchingBehavior == MatchOnlyUserAgentRules)
            matchUARules(collector);
        else
            matchAllRules(state, collector, matchingBehavior != MatchAllRulesExcludingSMIL);

        applyMatchedProperties(state, collector.matchedResult(), element);

        addContentAttrValuesToFeatures(state.contentAttrValues(), m_features);
    }
    {
        StyleAdjuster adjuster(state.cachedUAStyle(), m_document.inQuirksMode());
        adjuster.adjustRenderStyle(state.style(), state.parentStyle(), element);
    }

    if (element->hasTagName(bodyTag))
        document().textLinkColors().setTextColor(state.style()->visitedDependentColor(CSSPropertyColor));

    setAnimationUpdateIfNeeded(state, *element);

    return state.takeStyle();
}

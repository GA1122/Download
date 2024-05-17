PassRefPtr<RenderStyle> StyleResolver::styleForKeyframe(Element* e, const RenderStyle& elementStyle, const StyleKeyframe* keyframe)
{
    ASSERT(document().frame());
    ASSERT(documentSettings());

    if (e == document().documentElement())
        resetDirectionAndWritingModeOnDocument(document());
    StyleResolverState state(document(), e);

    MatchResult result;
    if (keyframe->properties())
        result.addMatchedProperties(keyframe->properties());

    ASSERT(!state.style());

    state.setStyle(RenderStyle::clone(&elementStyle));
    state.setLineHeightValue(0);

    state.fontBuilder().initForStyleResolve(state.document(), state.style(), state.useSVGZoomRules());

    bool inheritedOnly = false;
    if (keyframe->properties()) {
        applyMatchedProperties<AnimationProperties>(state, result, false, 0, result.matchedProperties.size() - 1, inheritedOnly);
        applyMatchedProperties<HighPriorityProperties>(state, result, false, 0, result.matchedProperties.size() - 1, inheritedOnly);
    }

    updateFont(state);

    if (state.lineHeightValue())
        StyleBuilder::applyProperty(CSSPropertyLineHeight, state, state.lineHeightValue());

    if (keyframe->properties())
        applyMatchedProperties<LowPriorityProperties>(state, result, false, 0, result.matchedProperties.size() - 1, inheritedOnly);

    updateFont(state);

    m_styleResourceLoader.loadPendingResources(state.style(), state.elementStyleResources());

    didAccess();

    return state.takeStyle();
}

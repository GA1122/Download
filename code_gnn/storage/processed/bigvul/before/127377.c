PassRefPtr<RenderStyle> StyleResolver::defaultStyleForElement()
{
    StyleResolverState state(document(), 0);
    state.setStyle(RenderStyle::create());
    state.fontBuilder().initForStyleResolve(document(), state.style(), state.useSVGZoomRules());
    state.style()->setLineHeight(RenderStyle::initialLineHeight());
    state.setLineHeightValue(0);
    state.fontBuilder().setInitial(state.style()->effectiveZoom());
    state.style()->font().update(fontSelector());
    return state.takeStyle();
}

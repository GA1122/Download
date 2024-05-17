void StyleResolver::applyAnimatedProperties(StyleResolverState& state, Element* animatingElement)
{
    if (!RuntimeEnabledFeatures::webAnimationsCSSEnabled() || !animatingElement)
        return;
    state.setAnimationUpdate(CSSAnimations::calculateUpdate(animatingElement, *state.style(), this));
    if (!state.animationUpdate())
        return;
    const AnimationEffect::CompositableValueMap& compositableValuesForAnimations = state.animationUpdate()->compositableValuesForAnimations();
    const AnimationEffect::CompositableValueMap& compositableValuesForTransitions = state.animationUpdate()->compositableValuesForTransitions();
    applyAnimatedProperties<HighPriorityProperties>(state, compositableValuesForAnimations);
    applyAnimatedProperties<HighPriorityProperties>(state, compositableValuesForTransitions);
    applyAnimatedProperties<LowPriorityProperties>(state, compositableValuesForAnimations);
    applyAnimatedProperties<LowPriorityProperties>(state, compositableValuesForTransitions);
}

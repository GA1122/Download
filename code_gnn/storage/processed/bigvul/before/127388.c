void StyleResolver::keyframeStylesForAnimation(Element* e, const RenderStyle& elementStyle, KeyframeList& list)
{
    ASSERT(!RuntimeEnabledFeatures::webAnimationsCSSEnabled());
    list.clear();

    if (!e || list.animationName().isEmpty())
        return;

    const StyleRuleKeyframes* keyframesRule = CSSAnimations::matchScopedKeyframesRule(this, e, list.animationName().impl());
    if (!keyframesRule)
        return;

    const Vector<RefPtr<StyleKeyframe> >& keyframes = keyframesRule->keyframes();
    for (unsigned i = 0; i < keyframes.size(); ++i) {
        const StyleKeyframe* keyframe = keyframes[i].get();

        KeyframeValue keyframeValue(0, 0);
        keyframeValue.setStyle(styleForKeyframe(e, elementStyle, keyframe));
        keyframeValue.addProperties(keyframe->properties());

        const Vector<double>& keys = keyframe->keys();
        for (size_t keyIndex = 0; keyIndex < keys.size(); ++keyIndex) {
            keyframeValue.setKey(keys[keyIndex]);
            list.insert(keyframeValue);
        }
    }

    int initialListSize = list.size();
    if (initialListSize > 0 && list[0].key()) {
        static StyleKeyframe* zeroPercentKeyframe;
        if (!zeroPercentKeyframe) {
            zeroPercentKeyframe = StyleKeyframe::create().leakRef();
            zeroPercentKeyframe->setKeyText("0%");
        }
        KeyframeValue keyframeValue(0, 0);
        keyframeValue.setStyle(styleForKeyframe(e, elementStyle, zeroPercentKeyframe));
        keyframeValue.addProperties(zeroPercentKeyframe->properties());
        list.insert(keyframeValue);
    }

    if (initialListSize > 0 && (list[list.size() - 1].key() != 1)) {
        static StyleKeyframe* hundredPercentKeyframe;
        if (!hundredPercentKeyframe) {
            hundredPercentKeyframe = StyleKeyframe::create().leakRef();
            hundredPercentKeyframe->setKeyText("100%");
        }
        KeyframeValue keyframeValue(1, 0);
        keyframeValue.setStyle(styleForKeyframe(e, elementStyle, hundredPercentKeyframe));
        keyframeValue.addProperties(hundredPercentKeyframe->properties());
        list.insert(keyframeValue);
    }
}

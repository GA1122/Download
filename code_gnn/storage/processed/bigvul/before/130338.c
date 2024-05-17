void RuleFeatureSet::updateInvalidationSetsForContentAttribute(const RuleData& ruleData)
{
    const StylePropertySet& propertySet = ruleData.rule()->properties();

    int propertyIndex = propertySet.findPropertyIndex(CSSPropertyContent);

    if (propertyIndex == -1)
        return;

    StylePropertySet::PropertyReference contentProperty = propertySet.propertyAt(propertyIndex);
    CSSValue* contentValue = contentProperty.value();

    if (!contentValue->isValueList())
        return;

    for (auto& item : toCSSValueList(*contentValue)) {
        if (!item->isFunctionValue())
            continue;
        CSSFunctionValue* functionValue = toCSSFunctionValue(item.get());
        if (functionValue->functionType() != CSSValueAttr)
            continue;
        ensureAttributeInvalidationSet(AtomicString(toCSSCustomIdentValue(functionValue->item(0))->value()), InvalidateDescendants).setInvalidatesSelf();
    }
}

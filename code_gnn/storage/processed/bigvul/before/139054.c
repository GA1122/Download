PassRefPtrWillBeRawPtr<FilterEffect> SVGFEColorMatrixElement::build(SVGFilterBuilder* filterBuilder, Filter* filter)
{
    FilterEffect* input1 = filterBuilder->getEffectById(AtomicString(m_in1->currentValue()->value()));

    if (!input1)
        return nullptr;

    Vector<float> filterValues;
    ColorMatrixType filterType = m_type->currentValue()->enumValue();

    if (!hasAttribute(SVGNames::valuesAttr)) {
        switch (filterType) {
        case FECOLORMATRIX_TYPE_MATRIX:
            for (size_t i = 0; i < 20; i++)
                filterValues.append((i % 6) ? 0 : 1);
            break;
        case FECOLORMATRIX_TYPE_HUEROTATE:
            filterValues.append(0);
            break;
        case FECOLORMATRIX_TYPE_SATURATE:
            filterValues.append(1);
            break;
        default:
            break;
        }
    } else {
        RefPtrWillBeRawPtr<SVGNumberList> values = m_values->currentValue();
        size_t size = values->length();

        if ((filterType == FECOLORMATRIX_TYPE_MATRIX && size != 20)
            || (filterType == FECOLORMATRIX_TYPE_HUEROTATE && size != 1)
            || (filterType == FECOLORMATRIX_TYPE_SATURATE && size != 1))
            return nullptr;

        filterValues = values->toFloatVector();
    }

    RefPtrWillBeRawPtr<FilterEffect> effect = FEColorMatrix::create(filter, filterType, filterValues);
    effect->inputEffects().append(input1);
    return effect.release();
}

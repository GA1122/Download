void HTMLInputElement::updateType()
{
    OwnPtr<InputType> newType = InputType::create(this, fastGetAttribute(typeAttr));
    bool hadType = m_hasType;
    m_hasType = true;
    if (m_inputType->formControlType() == newType->formControlType())
        return;

    if (hadType && !newType->canChangeFromAnotherType()) {
        setAttribute(typeAttr, type());
        return;
    }

    removeFromRadioButtonGroup();

    bool didStoreValue = m_inputType->storesValueSeparateFromAttribute();
    bool didRespectHeightAndWidth = m_inputType->shouldRespectHeightAndWidthAttributes();

    m_inputType->destroyShadowSubtree();

    bool wasAttached = attached();
    if (wasAttached)
        detach();

    m_inputType = newType.release();
    m_inputType->createShadowSubtree();

    bool hasTouchEventHandler = m_inputType->hasTouchEventHandler();
    if (hasTouchEventHandler != m_hasTouchEventHandler) {
        if (hasTouchEventHandler)
            document()->didAddTouchEventHandler(this);
        else
            document()->didRemoveTouchEventHandler(this);
        m_hasTouchEventHandler = hasTouchEventHandler;
    }

    setNeedsWillValidateCheck();

    bool willStoreValue = m_inputType->storesValueSeparateFromAttribute();

    if (didStoreValue && !willStoreValue && hasDirtyValue()) {
        setAttribute(valueAttr, m_valueIfDirty);
        m_valueIfDirty = String();
    }
    if (!didStoreValue && willStoreValue) {
        AtomicString valueString = fastGetAttribute(valueAttr);
        m_valueIfDirty = sanitizeValue(valueString);
    } else
        updateValueIfNeeded();

    setFormControlValueMatchesRenderer(false);
    m_inputType->updateInnerTextValue();

    m_wasModifiedByUser = false;

    if (didRespectHeightAndWidth != m_inputType->shouldRespectHeightAndWidthAttributes()) {
        ASSERT(elementData());
        if (const Attribute* height = getAttributeItem(heightAttr))
            attributeChanged(heightAttr, height->value());
        if (const Attribute* width = getAttributeItem(widthAttr))
            attributeChanged(widthAttr, width->value());
        if (const Attribute* align = getAttributeItem(alignAttr))
            attributeChanged(alignAttr, align->value());
    }

    if (wasAttached) {
        attach();
        if (document()->focusedNode() == this)
            updateFocusAppearance(true);
    }

    if (ElementShadow* elementShadow = shadowOfParentForDistribution(this))
        elementShadow->invalidateDistribution();

    setChangedSinceLastFormControlChangeEvent(false);

    addToRadioButtonGroup();

    setNeedsValidityCheck();
    notifyFormStateChanged();
}

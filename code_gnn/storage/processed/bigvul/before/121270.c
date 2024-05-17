bool HTMLInputElement::setupDateTimeChooserParameters(DateTimeChooserParameters& parameters)
{
    if (!document()->view())
        return false;

    parameters.type = type();
    parameters.minimum = minimum();
    parameters.maximum = maximum();
    parameters.required = isRequired();
    if (!RuntimeEnabledFeatures::langAttributeAwareFormControlUIEnabled())
        parameters.locale = defaultLanguage();
    else {
        AtomicString computedLocale = computeInheritedLanguage();
        parameters.locale = computedLocale.isEmpty() ? AtomicString(defaultLanguage()) : computedLocale;
    }

    StepRange stepRange = createStepRange(RejectAny);
    if (stepRange.hasStep()) {
        parameters.step = stepRange.step().toDouble();
        parameters.stepBase = stepRange.stepBase().toDouble();
    } else {
        parameters.step = 1.0;
        parameters.stepBase = 0;
    }

    parameters.anchorRectInRootView = document()->view()->contentsToRootView(pixelSnappedBoundingBox());
    parameters.currentValue = value();
    parameters.isAnchorElementRTL = computedStyle()->direction() == RTL;
    if (RuntimeEnabledFeatures::dataListElementEnabled()) {
        if (HTMLDataListElement* dataList = this->dataList()) {
            RefPtr<HTMLCollection> options = dataList->options();
            for (unsigned i = 0; HTMLOptionElement* option = toHTMLOptionElement(options->item(i)); ++i) {
                if (!isValidValue(option->value()))
                    continue;
                parameters.suggestionValues.append(sanitizeValue(option->value()));
                parameters.localizedSuggestionValues.append(localizeValue(option->value()));
                parameters.suggestionLabels.append(option->value() == option->label() ? String() : option->label());
            }
        }
    }
    return true;
}
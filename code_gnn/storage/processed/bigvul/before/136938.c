void HTMLInputElement::ParseAttribute(
    const AttributeModificationParams& params) {
  DCHECK(input_type_);
  DCHECK(input_type_view_);
  const QualifiedName& name = params.name;
  const AtomicString& value = params.new_value;

  if (name == nameAttr) {
    RemoveFromRadioButtonGroup();
    name_ = value;
    AddToRadioButtonGroup();
    TextControlElement::ParseAttribute(params);
  } else if (name == autocompleteAttr) {
    if (DeprecatedEqualIgnoringCase(value, "off")) {
      autocomplete_ = kOff;
    } else {
      if (value.IsEmpty())
        autocomplete_ = kUninitialized;
      else
        autocomplete_ = kOn;
    }
  } else if (name == typeAttr) {
    UpdateType();
  } else if (name == valueAttr) {
    if (!HasDirtyValue()) {
      if (input_type_->GetValueMode() == ValueMode::kValue)
        non_attribute_value_ = SanitizeValue(value);
      UpdatePlaceholderVisibility();
      SetNeedsStyleRecalc(
          kSubtreeStyleChange,
          StyleChangeReasonForTracing::FromAttribute(valueAttr));
    }
    needs_to_update_view_value_ = true;
    SetNeedsValidityCheck();
    input_type_->WarnIfValueIsInvalidAndElementIsVisible(value);
    input_type_->InRangeChanged();
    input_type_view_->ValueAttributeChanged();
  } else if (name == checkedAttr) {
    if ((!parsing_in_progress_ ||
         !GetDocument().GetFormController().HasFormStates()) &&
        !dirty_checkedness_) {
      setChecked(!value.IsNull());
      dirty_checkedness_ = false;
    }
    PseudoStateChanged(CSSSelector::kPseudoDefault);
  } else if (name == maxlengthAttr) {
    SetNeedsValidityCheck();
  } else if (name == minlengthAttr) {
    SetNeedsValidityCheck();
  } else if (name == sizeAttr) {
    unsigned size = 0;
    if (value.IsEmpty() || !ParseHTMLNonNegativeInteger(value, size) ||
        size == 0 || size > 0x7fffffffu)
      size = kDefaultSize;
    if (size_ != size) {
      size_ = size;
      if (GetLayoutObject())
        GetLayoutObject()
            ->SetNeedsLayoutAndPrefWidthsRecalcAndFullPaintInvalidation(
                LayoutInvalidationReason::kAttributeChanged);
    }
  } else if (name == altAttr) {
    input_type_view_->AltAttributeChanged();
  } else if (name == srcAttr) {
    input_type_view_->SrcAttributeChanged();
  } else if (name == usemapAttr || name == accesskeyAttr) {
  } else if (name == onsearchAttr) {
    SetAttributeEventListener(
        EventTypeNames::search,
        CreateAttributeEventListener(this, name, value, EventParameterName()));
  } else if (name == incrementalAttr) {
    UseCounter::Count(GetDocument(), WebFeature::kIncrementalAttribute);
  } else if (name == minAttr) {
    input_type_view_->MinOrMaxAttributeChanged();
    input_type_->SanitizeValueInResponseToMinOrMaxAttributeChange();
    input_type_->InRangeChanged();
    SetNeedsValidityCheck();
    UseCounter::Count(GetDocument(), WebFeature::kMinAttribute);
  } else if (name == maxAttr) {
    input_type_view_->MinOrMaxAttributeChanged();
    input_type_->SanitizeValueInResponseToMinOrMaxAttributeChange();
    input_type_->InRangeChanged();
    SetNeedsValidityCheck();
    UseCounter::Count(GetDocument(), WebFeature::kMaxAttribute);
  } else if (name == multipleAttr) {
    input_type_view_->MultipleAttributeChanged();
    SetNeedsValidityCheck();
  } else if (name == stepAttr) {
    input_type_view_->StepAttributeChanged();
    SetNeedsValidityCheck();
    UseCounter::Count(GetDocument(), WebFeature::kStepAttribute);
  } else if (name == patternAttr) {
    SetNeedsValidityCheck();
    UseCounter::Count(GetDocument(), WebFeature::kPatternAttribute);
  } else if (name == readonlyAttr) {
    TextControlElement::ParseAttribute(params);
    input_type_view_->ReadonlyAttributeChanged();
  } else if (name == listAttr) {
    has_non_empty_list_ = !value.IsEmpty();
    if (has_non_empty_list_) {
      ResetListAttributeTargetObserver();
      ListAttributeTargetChanged();
    }
    UseCounter::Count(GetDocument(), WebFeature::kListAttribute);
  } else if (name == webkitdirectoryAttr) {
    TextControlElement::ParseAttribute(params);
    UseCounter::Count(GetDocument(), WebFeature::kPrefixedDirectoryAttribute);
  } else {
    if (name == formactionAttr)
      LogUpdateAttributeIfIsolatedWorldAndInDocument("input", params);
    TextControlElement::ParseAttribute(params);
  }
  input_type_view_->AttributeChanged();
}

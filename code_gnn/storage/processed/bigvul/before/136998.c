void HTMLInputElement::UpdateType() {
  DCHECK(input_type_);
  DCHECK(input_type_view_);

  const AtomicString& new_type_name =
      InputType::NormalizeTypeName(FastGetAttribute(typeAttr));
  if (input_type_->FormControlType() == new_type_name)
    return;

  InputType* new_type = InputType::Create(*this, new_type_name);
  RemoveFromRadioButtonGroup();

  ValueMode old_value_mode = input_type_->GetValueMode();
  bool did_respect_height_and_width =
      input_type_->ShouldRespectHeightAndWidthAttributes();
  bool could_be_successful_submit_button = CanBeSuccessfulSubmitButton();

  input_type_view_->DestroyShadowSubtree();
  DropInnerEditorElement();
  LazyReattachIfAttached();

  if (input_type_->SupportsRequired() != new_type->SupportsRequired() &&
      IsRequired()) {
    PseudoStateChanged(CSSSelector::kPseudoRequired);
    PseudoStateChanged(CSSSelector::kPseudoOptional);
  }
  if (input_type_->SupportsReadOnly() != new_type->SupportsReadOnly()) {
    PseudoStateChanged(CSSSelector::kPseudoReadOnly);
    PseudoStateChanged(CSSSelector::kPseudoReadWrite);
  }
  if (input_type_->IsCheckable() != new_type->IsCheckable()) {
    PseudoStateChanged(CSSSelector::kPseudoChecked);
  }
  PseudoStateChanged(CSSSelector::kPseudoIndeterminate);
  if (input_type_->IsSteppable() || new_type->IsSteppable()) {
    PseudoStateChanged(CSSSelector::kPseudoInRange);
    PseudoStateChanged(CSSSelector::kPseudoOutOfRange);
  }

  bool placeholder_changed =
      input_type_->SupportsPlaceholder() != new_type->SupportsPlaceholder();

  has_been_password_field_ |= new_type_name == InputTypeNames::password;

  input_type_ = new_type;
  input_type_view_ = input_type_->CreateView();
  if (input_type_view_->NeedsShadowSubtree()) {
    EnsureUserAgentShadowRoot();
    CreateShadowSubtree();
  }

  SetNeedsWillValidateCheck();

  if (placeholder_changed) {
    UpdatePlaceholderText();
    UpdatePlaceholderVisibility();
    PseudoStateChanged(CSSSelector::kPseudoPlaceholderShown);
  }

  ValueMode new_value_mode = input_type_->GetValueMode();

  if (old_value_mode == ValueMode::kValue &&
      (new_value_mode == ValueMode::kDefault ||
       new_value_mode == ValueMode::kDefaultOn)) {
    if (HasDirtyValue())
      setAttribute(valueAttr, AtomicString(non_attribute_value_));
    non_attribute_value_ = String();
    has_dirty_value_ = false;
  }
  else if (old_value_mode != ValueMode::kValue &&
           new_value_mode == ValueMode::kValue) {
    AtomicString value_string = FastGetAttribute(valueAttr);
    input_type_->WarnIfValueIsInvalid(value_string);
    non_attribute_value_ = SanitizeValue(value_string);
    has_dirty_value_ = false;
  }
  else if (old_value_mode != ValueMode::kFilename &&
           new_value_mode == ValueMode::kFilename) {
    non_attribute_value_ = String();
    has_dirty_value_ = false;

  } else {
    if (!HasDirtyValue()) {
      String default_value = FastGetAttribute(valueAttr);
      if (!default_value.IsNull())
        input_type_->WarnIfValueIsInvalid(default_value);
    }

    if (new_value_mode == ValueMode::kValue) {
      String new_value = SanitizeValue(non_attribute_value_);
      if (!EqualIgnoringNullity(new_value, non_attribute_value_)) {
        if (HasDirtyValue())
          setValue(new_value);
        else
          SetNonDirtyValue(new_value);
      }
    }
  }

  needs_to_update_view_value_ = true;
  input_type_view_->UpdateView();

  if (did_respect_height_and_width !=
      input_type_->ShouldRespectHeightAndWidthAttributes()) {
    DCHECK(GetElementData());
    AttributeCollection attributes = AttributesWithoutUpdate();
    if (const Attribute* height = attributes.Find(heightAttr)) {
      TextControlElement::AttributeChanged(AttributeModificationParams(
          heightAttr, height->Value(), height->Value(),
          AttributeModificationReason::kDirectly));
    }
    if (const Attribute* width = attributes.Find(widthAttr)) {
      TextControlElement::AttributeChanged(
          AttributeModificationParams(widthAttr, width->Value(), width->Value(),
                                      AttributeModificationReason::kDirectly));
    }
    if (const Attribute* align = attributes.Find(alignAttr)) {
      TextControlElement::AttributeChanged(
          AttributeModificationParams(alignAttr, align->Value(), align->Value(),
                                      AttributeModificationReason::kDirectly));
    }
  }

  if (GetDocument().FocusedElement() == this)
    GetDocument().UpdateFocusAppearanceLater();

  ClearValueBeforeFirstUserEdit();

  AddToRadioButtonGroup();

  SetNeedsValidityCheck();
  if ((could_be_successful_submit_button || CanBeSuccessfulSubmitButton()) &&
      formOwner() && isConnected())
    formOwner()->InvalidateDefaultButtonStyle();
  NotifyFormStateChanged();
}

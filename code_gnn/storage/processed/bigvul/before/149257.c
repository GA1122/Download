String HTMLFormControlElement::formMethod() const {
  const AtomicString& form_method_attr = FastGetAttribute(kFormmethodAttr);
  if (form_method_attr.IsNull())
    return g_empty_string;
  return FormSubmission::Attributes::MethodString(
      FormSubmission::Attributes::ParseMethodType(form_method_attr));
}

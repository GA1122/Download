String HTMLFormControlElement::formEnctype() const {
  const AtomicString& form_enctype_attr = FastGetAttribute(kFormenctypeAttr);
  if (form_enctype_attr.IsNull())
    return g_empty_string;
  return FormSubmission::Attributes::ParseEncodingType(form_enctype_attr);
}

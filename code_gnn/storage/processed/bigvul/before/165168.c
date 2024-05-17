String HTMLFormElement::method() const {
  return FormSubmission::Attributes::MethodString(attributes_.Method());
}

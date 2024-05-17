double HTMLInputElement::valueAsDate(bool& is_null) const {
  double date = input_type_->ValueAsDate();
  is_null = !std::isfinite(date);
  return date;
}

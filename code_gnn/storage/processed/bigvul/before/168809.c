String CSSPaintValue::CustomCSSText() const {
  StringBuilder result;
  result.Append("paint(");
  result.Append(name_->CustomCSSText());
  for (const auto& variable_data : argument_variable_data_) {
    result.Append(", ");
    result.Append(variable_data.get()->TokenRange().Serialize());
  }
  result.Append(')');
  return result.ToString();
}

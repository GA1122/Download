CSSPaintValue::CSSPaintValue(
    CSSCustomIdentValue* name,
    Vector<scoped_refptr<CSSVariableData>>& variable_data)
    : CSSPaintValue(name) {
  argument_variable_data_.swap(variable_data);
}

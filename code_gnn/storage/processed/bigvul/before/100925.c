static int parseIndex(const Value* value) {
  int index = -1;
  string16 string16_index;
  double double_index;
  if (value->GetAsString(&string16_index)) {
    base::StringToInt(string16_index, &index);
  } else if (value->GetAsDouble(&double_index)) {
    index = static_cast<int>(double_index);
  } else {
    value->GetAsInteger(&index);
  }
  return index;
}

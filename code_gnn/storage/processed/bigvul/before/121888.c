void MaintainContentLengthPrefsWindow(base::ListValue* list, size_t length) {
  while (list->GetSize() > length)
    list->Remove(0, NULL);
  while (list->GetSize() < length)
    list->AppendString(base::Int64ToString(0));
   DCHECK_EQ(length, list->GetSize());
 }

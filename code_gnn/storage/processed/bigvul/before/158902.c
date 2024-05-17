bool CheckIfEditableFormTextArea(int flags, int form_type) {
  if (!!(flags & FPDF_FORMFLAG_READONLY))
    return false;
  if (form_type == FPDF_FORMFIELD_TEXTFIELD)
    return true;
  if (form_type == FPDF_FORMFIELD_COMBOBOX &&
      (!!(flags & FPDF_FORMFLAG_CHOICE_EDIT))) {
    return true;
  }
  return false;
}

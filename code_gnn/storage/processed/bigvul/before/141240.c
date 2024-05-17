void Document::SetStateForNewFormElements(const Vector<String>& state_vector) {
  if (!state_vector.size() && !form_controller_)
    return;
  GetFormController().SetStateForNewFormElements(state_vector);
}

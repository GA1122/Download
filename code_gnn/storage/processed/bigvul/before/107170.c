bool AutoFillManager::FindCachedForm(const FormData& form,
                                     FormStructure** form_structure) {
  *form_structure = NULL;
  for (std::vector<FormStructure*>::const_iterator iter =
       form_structures_.begin();
       iter != form_structures_.end(); ++iter) {
    if (**iter == form) {
      *form_structure = *iter;
      break;
    }
  }

  if (!(*form_structure))
    return false;

  return true;
}

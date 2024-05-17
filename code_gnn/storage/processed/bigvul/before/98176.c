void RemoveDuplicateElements(
    std::vector<string16>* elements, std::vector<int>* unique_ids) {
  DCHECK_EQ(elements->size(), unique_ids->size());

  std::vector<string16> elements_copy;
  std::vector<int> unique_ids_copy;
  for (size_t i = 0; i < elements->size(); ++i) {
    const string16& element = (*elements)[i];

    bool unique = true;
    for (std::vector<string16>::const_iterator copy_iter
             = elements_copy.begin();
         copy_iter != elements_copy.end(); ++copy_iter) {
      if (element == *copy_iter) {
        unique = false;
        break;
      }
    }

    if (unique) {
      elements_copy.push_back(element);
      unique_ids_copy.push_back((*unique_ids)[i]);
    }
  }

  elements->assign(elements_copy.begin(), elements_copy.end());
  unique_ids->assign(unique_ids_copy.begin(), unique_ids_copy.end());
}

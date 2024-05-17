void PasswordAutofillAgent::TryFixAutofilledForm(
    std::vector<WebFormControlElement>* control_elements) const {
  for (auto& element : *control_elements) {
    const unsigned element_id = element.UniqueRendererFormControlId();
    auto cached_element = autofilled_elements_cache_.find(element_id);
    if (cached_element == autofilled_elements_cache_.end())
      continue;

    const WebString& cached_value = cached_element->second;
    if (cached_value != element.SuggestedValue())
      element.SetSuggestedValue(cached_value);
  }
}

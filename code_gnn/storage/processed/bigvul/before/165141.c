HTMLFormElement::HTMLFormElement(Document& document)
    : HTMLElement(formTag, document),
      listed_elements_are_dirty_(false),
      image_elements_are_dirty_(false),
      has_elements_associated_by_parser_(false),
      has_elements_associated_by_form_attribute_(false),
      did_finish_parsing_children_(false),
      is_in_reset_function_(false),
      was_demoted_(false) {}

void FindFormElements(content::RenderFrame* render_frame,
                      const PasswordFormFillData& data,
                      bool ambiguous_or_empty_names,
                      FormElementsList* results) {
  DCHECK(results);

  blink::WebDocument doc = render_frame->GetWebFrame()->document();
  if (!doc.isHTMLDocument())
    return;

  if (data.origin != form_util::GetCanonicalOriginForDocument(doc))
    return;

  blink::WebVector<blink::WebFormElement> forms;
  doc.forms(forms);

  for (size_t i = 0; i < forms.size(); ++i) {
    blink::WebFormElement fe = forms[i];

    if (data.action != form_util::GetCanonicalActionForForm(fe))
      continue;

    std::vector<blink::WebFormControlElement> control_elements =
        form_util::ExtractAutofillableElementsInForm(fe);
    FormInputElementMap cur_map;
    if (FindFormInputElements(control_elements, data, ambiguous_or_empty_names,
                              &cur_map))
      results->push_back(cur_map);
  }
  if (data.action != data.origin)
    return;

  std::vector<blink::WebFormControlElement> control_elements =
      form_util::GetUnownedAutofillableFormFieldElements(doc.all(), nullptr);
  FormInputElementMap unowned_elements_map;
  if (FindFormInputElements(control_elements, data, ambiguous_or_empty_names,
                            &unowned_elements_map))
    results->push_back(unowned_elements_map);
}

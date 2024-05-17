  void TestFormFillFunctions(const char* html,
                             bool unowned,
                             const char* url_override,
                             const AutofillFieldCase* field_cases,
                             size_t number_of_field_cases,
                             FillFormFunction fill_form_function,
                             GetValueFunction get_value_function) {
    if (url_override)
      LoadHTMLWithUrlOverride(html, url_override);
    else
      LoadHTML(html);

    WebLocalFrame* web_frame = GetMainFrame();
    ASSERT_NE(nullptr, web_frame);

    FormCache form_cache(web_frame);
    std::vector<FormData> forms = form_cache.ExtractNewForms();
    ASSERT_EQ(1U, forms.size());

    WebInputElement input_element = GetInputElementById("firstname");

    FormData form_data;
    FormFieldData field;
    EXPECT_TRUE(FindFormAndFieldForFormControlElement(input_element, &form_data,
                                                      &field));
    if (!unowned) {
      EXPECT_EQ(ASCIIToUTF16("TestForm"), form_data.name);
      EXPECT_EQ(GURL("http://abc.com"), form_data.action);
    }

    const std::vector<FormFieldData>& fields = form_data.fields;
    ASSERT_EQ(number_of_field_cases, fields.size());

    FormFieldData expected;
    for (size_t i = 0; i < number_of_field_cases; ++i) {
      SCOPED_TRACE(base::StringPrintf("Verify initial value for field %s",
                                      field_cases[i].id_attribute));
      expected.form_control_type = field_cases[i].form_control_type;
      expected.max_length = expected.form_control_type == "text"
                                ? WebInputElement::DefaultMaxLength()
                                : 0;
      expected.id_attribute = ASCIIToUTF16(field_cases[i].id_attribute);
      expected.name = expected.id_attribute;
      expected.value = ASCIIToUTF16(field_cases[i].initial_value);
      if (expected.form_control_type == "text" ||
          expected.form_control_type == "month") {
        expected.label = ASCIIToUTF16(field_cases[i].initial_value);
      } else {
        expected.label.clear();
      }
      expected.autocomplete_attribute = field_cases[i].autocomplete_attribute;
      EXPECT_FORM_FIELD_DATA_EQUALS(expected, fields[i]);
      form_data.fields[i].value = ASCIIToUTF16(field_cases[i].autofill_value);
      form_data.fields[i].is_autofilled = field_cases[i].should_be_autofilled;
    }

    fill_form_function(form_data, input_element);

    for (size_t i = 0; i < number_of_field_cases; ++i) {
      ValidateFilledField(field_cases[i], get_value_function);
    }
  }

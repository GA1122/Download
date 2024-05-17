  void TestFillFormWithPlaceholderValues(const char* html,
                                         const char* placeholder_firstname,
                                         const char* placeholder_lastname,
                                         const char* placeholder_email) {
    LoadHTML(html);
    WebLocalFrame* web_frame = GetMainFrame();
    ASSERT_NE(nullptr, web_frame);

    FormCache form_cache(web_frame);
    std::vector<FormData> forms = form_cache.ExtractNewForms();
    ASSERT_EQ(1U, forms.size());

    WebInputElement input_element = GetInputElementById("firstname");
    WebFormElement form_element = input_element.Form();
    std::vector<WebFormControlElement> control_elements =
        ExtractAutofillableElementsInForm(form_element);

    ASSERT_EQ(3U, control_elements.size());
    control_elements[0].SetValue(
        WebString::FromUTF16(base::char16(base::i18n::kLeftToRightMark) +
                             ASCIIToUTF16("first name")));
    control_elements[1].SetValue(
        WebString::FromUTF16(ASCIIToUTF16("LAST NAME")));
    control_elements[2].SetValue(
        WebString::FromUTF16(ASCIIToUTF16("john@smith.com")));
    FormData form;
    FormFieldData field;
    EXPECT_TRUE(
        FindFormAndFieldForFormControlElement(input_element, &form, &field));
    EXPECT_EQ(GetCanonicalOriginForDocument(web_frame->GetDocument()),
              form.origin);
    EXPECT_EQ(ASCIIToUTF16("TestForm"), form.name);
    EXPECT_EQ(GURL("http://abc.com"), form.action);

    const std::vector<FormFieldData>& fields = form.fields;
    ASSERT_EQ(3U, fields.size());

    form.fields[0].value = ASCIIToUTF16("Wyatt");
    form.fields[1].value = ASCIIToUTF16("Earpagus");
    form.fields[2].value = ASCIIToUTF16("susan@smith.com");
    form.fields[0].is_autofilled = true;
    form.fields[1].is_autofilled = true;
    form.fields[2].is_autofilled = false;
    PreviewForm(form, input_element);
    EXPECT_EQ(5, input_element.SelectionStart());
    EXPECT_EQ(5, input_element.SelectionEnd());

    FillForm(form, input_element);

    FormData form2;
    FormFieldData field2;
    EXPECT_TRUE(
        FindFormAndFieldForFormControlElement(input_element, &form2, &field2));
    EXPECT_EQ(GetCanonicalOriginForDocument(web_frame->GetDocument()),
              form2.origin);
    EXPECT_EQ(ASCIIToUTF16("TestForm"), form2.name);
    EXPECT_EQ(GURL("http://abc.com"), form2.action);

    const std::vector<FormFieldData>& fields2 = form2.fields;
    ASSERT_EQ(3U, fields2.size());

    FormFieldData expected;
    expected.form_control_type = "text";
    expected.max_length = WebInputElement::DefaultMaxLength();

    expected.id_attribute = ASCIIToUTF16("firstname");
    expected.name = expected.id_attribute;
    expected.value = ASCIIToUTF16("Wyatt");
    if (placeholder_firstname) {
      expected.label = ASCIIToUTF16(placeholder_firstname);
      expected.placeholder = ASCIIToUTF16(placeholder_firstname);
    } else {
      expected.label.clear();
      expected.placeholder.clear();
    }
    expected.is_autofilled = true;
    EXPECT_FORM_FIELD_DATA_EQUALS(expected, fields2[0]);

    expected.id_attribute = ASCIIToUTF16("lastname");
    expected.name = expected.id_attribute;
    expected.value = ASCIIToUTF16("Earpagus");
    if (placeholder_lastname) {
      expected.label = ASCIIToUTF16(placeholder_lastname);
      expected.placeholder = ASCIIToUTF16(placeholder_lastname);
    } else {
      expected.label.clear();
      expected.placeholder.clear();
    }
    expected.is_autofilled = true;
    EXPECT_FORM_FIELD_DATA_EQUALS(expected, fields2[1]);

    expected.id_attribute = ASCIIToUTF16("email");
    expected.name = expected.id_attribute;
    expected.value = ASCIIToUTF16("john@smith.com");
    if (placeholder_email) {
      expected.label = ASCIIToUTF16(placeholder_email);
      expected.placeholder = ASCIIToUTF16(placeholder_email);
    } else {
      expected.label.clear();
      expected.placeholder.clear();
    }
    expected.is_autofilled = false;
    EXPECT_FORM_FIELD_DATA_EQUALS(expected, fields2[2]);

    EXPECT_EQ(5, input_element.SelectionStart());
    EXPECT_EQ(5, input_element.SelectionEnd());
  }

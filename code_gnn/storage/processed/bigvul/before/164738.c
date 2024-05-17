  void TestFillFormJSModifiesUserInputValue(const char* html,
                                            const char* placeholder_creditcard,
                                            const char* placeholder_expiration,
                                            const char* placeholder_name) {
    LoadHTML(html);
    WebLocalFrame* web_frame = GetMainFrame();
    ASSERT_NE(nullptr, web_frame);

    FormCache form_cache(web_frame);
    std::vector<FormData> forms = form_cache.ExtractNewForms();
    ASSERT_EQ(1U, forms.size());

    WebInputElement input_element = GetInputElementById("cc");
    WebFormElement form_element = input_element.Form();
    std::vector<WebFormControlElement> control_elements =
        ExtractAutofillableElementsInForm(form_element);

    ASSERT_EQ(3U, control_elements.size());
    control_elements[0].SetValue(
        WebString::FromUTF16(ASCIIToUTF16("____-____-____-____")));
    control_elements[1].SetValue(WebString::FromUTF16(ASCIIToUTF16("____/__")));
    control_elements[2].SetValue(
        WebString::FromUTF16(ASCIIToUTF16("john smith")));
    control_elements[2].SetUserHasEditedTheFieldForTest();

    ExecuteJavaScriptForTests(
        "document.getElementById('name').value = 'John Smith';");

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

    form.fields[0].value = ASCIIToUTF16("1111-2222-3333-4444");
    form.fields[1].value = ASCIIToUTF16("03/2030");
    form.fields[2].value = ASCIIToUTF16("Susan Smith");
    form.fields[0].is_autofilled = true;
    form.fields[1].is_autofilled = true;
    form.fields[2].is_autofilled = true;
    PreviewForm(form, input_element);
    EXPECT_EQ(19, input_element.SelectionStart());
    EXPECT_EQ(19, input_element.SelectionEnd());

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

    expected.id_attribute = ASCIIToUTF16("cc");
    expected.name = expected.id_attribute;
    expected.value = ASCIIToUTF16("1111-2222-3333-4444");
    if (placeholder_creditcard) {
      expected.label = ASCIIToUTF16(placeholder_creditcard);
      expected.placeholder = ASCIIToUTF16(placeholder_creditcard);
    } else {
      expected.label.clear();
      expected.placeholder.clear();
    }
    expected.is_autofilled = true;
    EXPECT_FORM_FIELD_DATA_EQUALS(expected, fields2[0]);

    expected.id_attribute = ASCIIToUTF16("expiration_date");
    expected.name = expected.id_attribute;
    expected.value = ASCIIToUTF16("03/2030");
    if (placeholder_expiration) {
      expected.label = ASCIIToUTF16(placeholder_expiration);
      expected.placeholder = ASCIIToUTF16(placeholder_expiration);
    } else {
      expected.label.clear();
      expected.placeholder.clear();
    }
    expected.is_autofilled = true;
    EXPECT_FORM_FIELD_DATA_EQUALS(expected, fields2[1]);

    expected.id_attribute = ASCIIToUTF16("name");
    expected.name = expected.id_attribute;
    expected.value = ASCIIToUTF16("John Smith");
    if (placeholder_name) {
      expected.label = ASCIIToUTF16(placeholder_name);
      expected.placeholder = ASCIIToUTF16(placeholder_name);
    } else {
      expected.label.clear();
      expected.placeholder.clear();
    }
    expected.is_autofilled = false;
    EXPECT_FORM_FIELD_DATA_EQUALS(expected, fields2[2]);

    EXPECT_EQ(19, input_element.SelectionStart());
    EXPECT_EQ(19, input_element.SelectionEnd());
  }

  void TestFillFormAndModifyValues(const char* html,
                                   const char* placeholder_firstname,
                                   const char* placeholder_lastname,
                                   const char* placeholder_phone,
                                   const char* placeholder_creditcard,
                                   const char* placeholder_city,
                                   const char* placeholder_state) {
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

    ASSERT_EQ(6U, control_elements.size());
    control_elements[0].SetValue(WebString::FromUTF16(
        base::char16(base::i18n::kLeftToRightMark) + ASCIIToUTF16("     ")));

    control_elements[1].SetValue(WebString::FromUTF16(ASCIIToUTF16("Earp")));
    control_elements[1].SetUserHasEditedTheFieldForTest();

    control_elements[2].SetValue(
        WebString::FromUTF16(ASCIIToUTF16("(___)-___-____")));

    control_elements[3].SetValue(
        WebString::FromUTF16(ASCIIToUTF16("____-____-____-____")));

    control_elements[4].SetValue(
        WebString::FromUTF16(ASCIIToUTF16("Enter your city..")));

    control_elements[5].SetValue(WebString::FromUTF16(ASCIIToUTF16("AK")));
    control_elements[5].SetUserHasEditedTheFieldForTest();

    FormData form;
    FormFieldData field;
    EXPECT_TRUE(
        FindFormAndFieldForFormControlElement(input_element, &form, &field));
    EXPECT_EQ(GetCanonicalOriginForDocument(web_frame->GetDocument()),
              form.origin);
    EXPECT_EQ(ASCIIToUTF16("TestForm"), form.name);
    EXPECT_EQ(GURL("http://abc.com"), form.action);

    const std::vector<FormFieldData>& fields = form.fields;
    ASSERT_EQ(6U, fields.size());

    form.fields[0].value = ASCIIToUTF16("Wyatt");
    form.fields[1].value = ASCIIToUTF16("Earpagus");
    form.fields[2].value = ASCIIToUTF16("888-123-4567");
    form.fields[3].value = ASCIIToUTF16("1111-2222-3333-4444");
    form.fields[4].value = ASCIIToUTF16("Montreal");
    form.fields[5].value = ASCIIToUTF16("AA");
    form.fields[0].is_autofilled = true;
    form.fields[1].is_autofilled = true;
    form.fields[2].is_autofilled = true;
    form.fields[3].is_autofilled = true;
    form.fields[4].is_autofilled = true;
    form.fields[5].is_autofilled = true;
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
    ASSERT_EQ(6U, fields2.size());

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
    expected.value = ASCIIToUTF16("Earp");
    if (placeholder_lastname) {
      expected.label = ASCIIToUTF16(placeholder_lastname);
      expected.placeholder = ASCIIToUTF16(placeholder_lastname);
    } else {
      expected.label.clear();
      expected.placeholder.clear();
    }
    expected.is_autofilled = false;
    EXPECT_FORM_FIELD_DATA_EQUALS(expected, fields2[1]);

    expected.id_attribute = ASCIIToUTF16("phone");
    expected.name = expected.id_attribute;
    expected.value = ASCIIToUTF16("888-123-4567");
    if (placeholder_phone) {
      expected.label = ASCIIToUTF16(placeholder_phone);
      expected.placeholder = ASCIIToUTF16(placeholder_phone);
    } else {
      expected.label.clear();
      expected.placeholder.clear();
    }
    expected.is_autofilled = true;
    EXPECT_FORM_FIELD_DATA_EQUALS(expected, fields2[2]);

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
    EXPECT_FORM_FIELD_DATA_EQUALS(expected, fields2[3]);

    expected.id_attribute = ASCIIToUTF16("city");
    expected.name = expected.id_attribute;
    expected.value =
        base::FeatureList::IsEnabled(features::kAutofillPrefilledFields)
            ? ASCIIToUTF16("Montreal")
            : control_elements[4].Value().Utf16();
    if (placeholder_city) {
      expected.label = ASCIIToUTF16(placeholder_city);
      expected.placeholder = ASCIIToUTF16(placeholder_city);
    } else {
      expected.label.clear();
      expected.placeholder.clear();
    }
    expected.is_autofilled =
        base::FeatureList::IsEnabled(features::kAutofillPrefilledFields);
    EXPECT_FORM_FIELD_DATA_EQUALS(expected, fields2[4]);

    expected.form_control_type = "select-one";
    expected.id_attribute = ASCIIToUTF16("state");
    expected.name_attribute = ASCIIToUTF16("state");
    expected.name = expected.name_attribute;
    expected.value =
        base::FeatureList::IsEnabled(features::kAutofillPrefilledFields)
            ? ASCIIToUTF16("AA")
            : control_elements[5].Value().Utf16();
    if (placeholder_state) {
      expected.label = ASCIIToUTF16(placeholder_state);
      expected.placeholder = ASCIIToUTF16(placeholder_state);
    } else {
      expected.label.clear();
      expected.placeholder.clear();
    }
    expected.is_autofilled =
        base::FeatureList::IsEnabled(features::kAutofillPrefilledFields);
    expected.max_length = 0;
    EXPECT_FORM_FIELD_DATA_EQUALS(expected, fields2[5]);

    EXPECT_EQ(5, input_element.SelectionStart());
    EXPECT_EQ(5, input_element.SelectionEnd());
  }

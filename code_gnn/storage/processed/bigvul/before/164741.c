  void TestFillFormNonEmptyField(const char* html,
                                 bool unowned,
                                 const char* initial_lastname,
                                 const char* initial_email,
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

    input_element.SetValue(WebString::FromASCII("Wy"));

    FormData form;
    FormFieldData field;
    EXPECT_TRUE(
        FindFormAndFieldForFormControlElement(input_element, &form, &field));
    EXPECT_EQ(GetCanonicalOriginForDocument(web_frame->GetDocument()),
              form.origin);
    if (!unowned) {
      EXPECT_EQ(ASCIIToUTF16("TestForm"), form.name);
      EXPECT_EQ(GURL("http://abc.com"), form.action);
    }

    const std::vector<FormFieldData>& fields = form.fields;
    ASSERT_EQ(3U, fields.size());

    FormFieldData expected;
    expected.form_control_type = "text";
    expected.max_length = WebInputElement::DefaultMaxLength();

    expected.id_attribute = ASCIIToUTF16("firstname");
    expected.name = expected.id_attribute;
    expected.value = ASCIIToUTF16("Wy");
    if (placeholder_firstname) {
      expected.label = ASCIIToUTF16(placeholder_firstname);
      expected.placeholder = ASCIIToUTF16(placeholder_firstname);
    }
    expected.is_autofilled = false;
    EXPECT_FORM_FIELD_DATA_EQUALS(expected, fields[0]);

    expected.id_attribute = ASCIIToUTF16("lastname");
    expected.name = expected.id_attribute;
    if (initial_lastname) {
      expected.label = ASCIIToUTF16(initial_lastname);
      expected.value = ASCIIToUTF16(initial_lastname);
    } else if (placeholder_lastname) {
      expected.label = ASCIIToUTF16(placeholder_lastname);
      expected.placeholder = ASCIIToUTF16(placeholder_lastname);
      expected.value = ASCIIToUTF16(placeholder_lastname);
    } else {
      expected.label.clear();
      expected.value.clear();
    }
    expected.is_autofilled = false;
    EXPECT_FORM_FIELD_DATA_EQUALS(expected, fields[1]);

    expected.id_attribute = ASCIIToUTF16("email");
    expected.name = expected.id_attribute;
    if (initial_email) {
      expected.label = ASCIIToUTF16(initial_email);
      expected.value = ASCIIToUTF16(initial_email);
    } else if (placeholder_email) {
      expected.label = ASCIIToUTF16(placeholder_email);
      expected.placeholder = ASCIIToUTF16(placeholder_email);
      expected.value = ASCIIToUTF16(placeholder_email);
    } else {
      expected.label.clear();
      expected.value.clear();
    }
    expected.is_autofilled = false;
    EXPECT_FORM_FIELD_DATA_EQUALS(expected, fields[2]);

    form.fields[0].value = ASCIIToUTF16("Wyatt");
    form.fields[1].value = ASCIIToUTF16("Earp");
    form.fields[2].value = ASCIIToUTF16("wyatt@example.com");
    form.fields[0].is_autofilled = true;
    form.fields[1].is_autofilled = true;
    form.fields[2].is_autofilled = true;
    PreviewForm(form, input_element);
    EXPECT_EQ(2, input_element.SelectionStart());
    EXPECT_EQ(2, input_element.SelectionEnd());

    FillForm(form, input_element);

    FormData form2;
    FormFieldData field2;
    EXPECT_TRUE(
        FindFormAndFieldForFormControlElement(input_element, &form2, &field2));
    EXPECT_EQ(GetCanonicalOriginForDocument(web_frame->GetDocument()),
              form2.origin);
    if (!unowned) {
      EXPECT_EQ(ASCIIToUTF16("TestForm"), form2.name);
      EXPECT_EQ(GURL("http://abc.com"), form2.action);
    }

    const std::vector<FormFieldData>& fields2 = form2.fields;
    ASSERT_EQ(3U, fields2.size());

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
    expected.is_autofilled = true;
    EXPECT_FORM_FIELD_DATA_EQUALS(expected, fields2[1]);

    expected.id_attribute = ASCIIToUTF16("email");
    expected.name = expected.id_attribute;
    expected.value = ASCIIToUTF16("wyatt@example.com");
    if (placeholder_email) {
      expected.label = ASCIIToUTF16(placeholder_email);
      expected.placeholder = ASCIIToUTF16(placeholder_email);
    } else {
      expected.label.clear();
      expected.placeholder.clear();
    }
    expected.is_autofilled = true;
    EXPECT_FORM_FIELD_DATA_EQUALS(expected, fields2[2]);

    EXPECT_EQ(5, input_element.SelectionStart());
    EXPECT_EQ(5, input_element.SelectionEnd());
  }

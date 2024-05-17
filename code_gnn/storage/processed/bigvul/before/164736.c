  void TestFillFormEmptyFormNames(const char* html, bool unowned) {
    LoadHTML(html);
    WebLocalFrame* web_frame = GetMainFrame();
    ASSERT_NE(nullptr, web_frame);

    FormCache form_cache(web_frame);
    std::vector<FormData> forms = form_cache.ExtractNewForms();
    const size_t expected_size = unowned ? 1 : 2;
    ASSERT_EQ(expected_size, forms.size());

    WebInputElement input_element = GetInputElementById("apple");

    FormData form;
    FormFieldData field;
    EXPECT_TRUE(
        FindFormAndFieldForFormControlElement(input_element, &form, &field));
    EXPECT_EQ(GetCanonicalOriginForDocument(web_frame->GetDocument()),
              form.origin);
    if (!unowned) {
      EXPECT_TRUE(form.name.empty());
      EXPECT_EQ(GURL("http://abc.com"), form.action);
    }

    const std::vector<FormFieldData>& fields = form.fields;
    const size_t unowned_offset = unowned ? 3 : 0;
    ASSERT_EQ(unowned_offset + 3, fields.size());

    FormFieldData expected;
    expected.form_control_type = "text";
    expected.max_length = WebInputElement::DefaultMaxLength();

    expected.id_attribute = ASCIIToUTF16("apple");
    expected.name = expected.id_attribute;
    expected.is_autofilled = false;
    EXPECT_FORM_FIELD_DATA_EQUALS(expected, fields[unowned_offset]);

    expected.id_attribute = ASCIIToUTF16("banana");
    expected.name = expected.id_attribute;
    expected.is_autofilled = false;
    EXPECT_FORM_FIELD_DATA_EQUALS(expected, fields[unowned_offset + 1]);

    expected.id_attribute = ASCIIToUTF16("cantelope");
    expected.name = expected.id_attribute;
    expected.is_autofilled = false;
    EXPECT_FORM_FIELD_DATA_EQUALS(expected, fields[unowned_offset + 2]);

    form.fields[unowned_offset + 0].value = ASCIIToUTF16("Red");
    form.fields[unowned_offset + 1].value = ASCIIToUTF16("Yellow");
    form.fields[unowned_offset + 2].value = ASCIIToUTF16("Also Yellow");
    form.fields[unowned_offset + 0].is_autofilled = true;
    form.fields[unowned_offset + 1].is_autofilled = true;
    form.fields[unowned_offset + 2].is_autofilled = true;
    FillForm(form, input_element);

    FormData form2;
    FormFieldData field2;
    EXPECT_TRUE(
        FindFormAndFieldForFormControlElement(input_element, &form2, &field2));
    EXPECT_EQ(GetCanonicalOriginForDocument(web_frame->GetDocument()),
              form2.origin);
    if (!unowned) {
      EXPECT_TRUE(form2.name.empty());
      EXPECT_EQ(GURL("http://abc.com"), form2.action);
    }

    const std::vector<FormFieldData>& fields2 = form2.fields;
    ASSERT_EQ(unowned_offset + 3, fields2.size());

    expected.id_attribute = ASCIIToUTF16("apple");
    expected.name = expected.id_attribute;
    expected.value = ASCIIToUTF16("Red");
    expected.is_autofilled = true;
    EXPECT_FORM_FIELD_DATA_EQUALS(expected, fields2[unowned_offset + 0]);

    expected.id_attribute = ASCIIToUTF16("banana");
    expected.name = expected.id_attribute;
    expected.value = ASCIIToUTF16("Yellow");
    expected.is_autofilled = true;
    EXPECT_FORM_FIELD_DATA_EQUALS(expected, fields2[unowned_offset + 1]);

    expected.id_attribute = ASCIIToUTF16("cantelope");
    expected.name = expected.id_attribute;
    expected.value = ASCIIToUTF16("Also Yellow");
    expected.is_autofilled = true;
    EXPECT_FORM_FIELD_DATA_EQUALS(expected, fields2[unowned_offset + 2]);
  }

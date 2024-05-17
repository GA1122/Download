  void TestFindFormForInputElement(const char* html, bool unowned) {
    LoadHTML(html);
    WebLocalFrame* web_frame = GetMainFrame();
    ASSERT_NE(nullptr, web_frame);

    FormCache form_cache(web_frame);
    std::vector<FormData> forms = form_cache.ExtractNewForms();
    ASSERT_EQ(1U, forms.size());

    WebInputElement input_element = GetInputElementById("firstname");

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
    ASSERT_EQ(4U, fields.size());

    FormFieldData expected;
    expected.form_control_type = "text";
    expected.max_length = WebInputElement::DefaultMaxLength();

    expected.id_attribute = ASCIIToUTF16("firstname");
    expected.name = expected.id_attribute;
    expected.value = ASCIIToUTF16("John");
    expected.label = ASCIIToUTF16("John");
    EXPECT_FORM_FIELD_DATA_EQUALS(expected, fields[0]);
    EXPECT_FORM_FIELD_DATA_EQUALS(expected, field);

    expected.id_attribute = ASCIIToUTF16("lastname");
    expected.name = expected.id_attribute;
    expected.value = ASCIIToUTF16("Smith");
    expected.label = ASCIIToUTF16("Smith");
    EXPECT_FORM_FIELD_DATA_EQUALS(expected, fields[1]);

    expected.id_attribute = ASCIIToUTF16("email");
    expected.name = expected.id_attribute;
    expected.value = ASCIIToUTF16("john@example.com");
    expected.label = ASCIIToUTF16("john@example.com");
    expected.autocomplete_attribute = "off";
    EXPECT_FORM_FIELD_DATA_EQUALS(expected, fields[2]);
    expected.autocomplete_attribute.clear();

    expected.id_attribute = ASCIIToUTF16("phone");
    expected.name = expected.id_attribute;
    expected.value = ASCIIToUTF16("1.800.555.1234");
    expected.label = ASCIIToUTF16("1.800.555.1234");
    EXPECT_FORM_FIELD_DATA_EQUALS(expected, fields[3]);
  }

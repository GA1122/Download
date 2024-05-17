  void TestFindFormForTextAreaElement(const char* html, bool unowned) {
    LoadHTML(html);
    WebLocalFrame* web_frame = GetMainFrame();
    ASSERT_NE(nullptr, web_frame);

    FormCache form_cache(web_frame);
    std::vector<FormData> forms = form_cache.ExtractNewForms();
    ASSERT_EQ(1U, forms.size());

    WebElement element =
        web_frame->GetDocument().GetElementById("street-address");
    WebFormControlElement textarea_element =
        element.To<WebFormControlElement>();

    FormData form;
    FormFieldData field;
    EXPECT_TRUE(
        FindFormAndFieldForFormControlElement(textarea_element, &form, &field));
    EXPECT_EQ(GetCanonicalOriginForDocument(web_frame->GetDocument()),
              form.origin);
    if (!unowned) {
      EXPECT_EQ(ASCIIToUTF16("TestForm"), form.name);
      EXPECT_EQ(GURL("http://abc.com"), form.action);
    }

    const std::vector<FormFieldData>& fields = form.fields;
    ASSERT_EQ(4U, fields.size());

    FormFieldData expected;

    expected.id_attribute = ASCIIToUTF16("firstname");
    expected.name = expected.id_attribute;
    expected.value = ASCIIToUTF16("John");
    expected.label = ASCIIToUTF16("John");
    expected.form_control_type = "text";
    expected.max_length = WebInputElement::DefaultMaxLength();
    EXPECT_FORM_FIELD_DATA_EQUALS(expected, fields[0]);

    expected.id_attribute = ASCIIToUTF16("lastname");
    expected.name = expected.id_attribute;
    expected.value = ASCIIToUTF16("Smith");
    expected.label = ASCIIToUTF16("Smith");
    expected.form_control_type = "text";
    expected.max_length = WebInputElement::DefaultMaxLength();
    EXPECT_FORM_FIELD_DATA_EQUALS(expected, fields[1]);

    expected.id_attribute = ASCIIToUTF16("email");
    expected.name = expected.id_attribute;
    expected.value = ASCIIToUTF16("john@example.com");
    expected.label = ASCIIToUTF16("john@example.com");
    expected.autocomplete_attribute = "off";
    expected.form_control_type = "text";
    expected.max_length = WebInputElement::DefaultMaxLength();
    EXPECT_FORM_FIELD_DATA_EQUALS(expected, fields[2]);
    expected.autocomplete_attribute.clear();

    expected.id_attribute = ASCIIToUTF16("street-address");
    expected.name = expected.id_attribute;
    expected.value = ASCIIToUTF16("123 Fantasy Ln.\nApt. 42");
    expected.label.clear();
    expected.form_control_type = "textarea";
    expected.max_length = 0;
    EXPECT_FORM_FIELD_DATA_EQUALS(expected, fields[3]);
    EXPECT_FORM_FIELD_DATA_EQUALS(expected, field);
  }

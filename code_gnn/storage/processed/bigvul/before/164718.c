  void ExpectLabelsAndTypes(const char* html,
                            const std::vector<base::string16>& id_attributes,
                            const std::vector<base::string16>& name_attributes,
                            const std::vector<base::string16>& labels,
                            const std::vector<base::string16>& names,
                            const std::vector<base::string16>& values,
                            const std::vector<std::string>& control_types) {
    ASSERT_EQ(labels.size(), id_attributes.size());
    ASSERT_EQ(labels.size(), name_attributes.size());
    ASSERT_EQ(labels.size(), names.size());
    ASSERT_EQ(labels.size(), values.size());
    ASSERT_EQ(labels.size(), control_types.size());

    LoadHTML(html);

    WebLocalFrame* web_frame = GetMainFrame();
    ASSERT_NE(nullptr, web_frame);

    FormCache form_cache(web_frame);
    std::vector<FormData> forms = form_cache.ExtractNewForms();
    ASSERT_EQ(1U, forms.size());

    const FormData& form = forms[0];
    EXPECT_EQ(ASCIIToUTF16("TestForm"), form.name);
    EXPECT_EQ(GetCanonicalOriginForDocument(web_frame->GetDocument()),
              form.origin);
    EXPECT_EQ(GURL("http://cnn.com"), form.action);

    const std::vector<FormFieldData>& fields = form.fields;
    ASSERT_EQ(labels.size(), fields.size());
    for (size_t i = 0; i < labels.size(); ++i) {
      int max_length =
          control_types[i] == "text" ? WebInputElement::DefaultMaxLength() : 0;
      FormFieldData expected;
      expected.id_attribute = id_attributes[i];
      expected.name_attribute = name_attributes[i];
      expected.label = labels[i];
      expected.name = names[i];
      expected.value = values[i];
      expected.form_control_type = control_types[i];
      expected.max_length = max_length;
      SCOPED_TRACE(base::StringPrintf("i: %" PRIuS, i));
      EXPECT_FORM_FIELD_DATA_EQUALS(expected, fields[i]);
    }
  }

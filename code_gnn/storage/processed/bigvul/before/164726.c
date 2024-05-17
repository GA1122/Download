  void TestClearOnlyAutofilledFields(const char* html) {
    LoadHTML(html);

    WebLocalFrame* web_frame = GetMainFrame();
    ASSERT_NE(nullptr, web_frame);

    FormCache form_cache(web_frame);
    std::vector<FormData> forms = form_cache.ExtractNewForms();
    ASSERT_EQ(1U, forms.size());

    WebInputElement firstname = GetInputElementById("firstname");
    firstname.SetAutofillState(WebAutofillState::kNotFilled);
    WebInputElement lastname = GetInputElementById("lastname");
    lastname.SetAutofillState(WebAutofillState::kAutofilled);
    WebInputElement email = GetInputElementById("email");
    email.SetAutofillState(WebAutofillState::kAutofilled);
    WebInputElement phone = GetInputElementById("phone");
    phone.SetAutofillState(WebAutofillState::kAutofilled);

    EXPECT_TRUE(form_cache.ClearSectionWithElement(firstname));

    EXPECT_EQ(ASCIIToUTF16("Wyatt"), firstname.Value().Utf16());
    EXPECT_TRUE(firstname.SuggestedValue().IsEmpty());
    EXPECT_FALSE(firstname.IsAutofilled());
    EXPECT_TRUE(lastname.Value().IsEmpty());
    EXPECT_TRUE(lastname.SuggestedValue().IsEmpty());
    EXPECT_FALSE(lastname.IsAutofilled());
    EXPECT_TRUE(email.Value().IsEmpty());
    EXPECT_TRUE(email.SuggestedValue().IsEmpty());
    EXPECT_FALSE(email.IsAutofilled());
    EXPECT_TRUE(phone.Value().IsEmpty());
    EXPECT_TRUE(phone.SuggestedValue().IsEmpty());
    EXPECT_FALSE(phone.IsAutofilled());
  }

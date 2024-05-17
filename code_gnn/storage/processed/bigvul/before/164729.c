  void TestClearPreviewedFormWithNonEmptyInitiatingNode(const char* html) {
    LoadHTML(html);
    WebLocalFrame* web_frame = GetMainFrame();
    ASSERT_NE(nullptr, web_frame);

    FormCache form_cache(web_frame);
    std::vector<FormData> forms = form_cache.ExtractNewForms();
    ASSERT_EQ(1U, forms.size());

    WebInputElement firstname = GetInputElementById("firstname");
    firstname.SetAutofillState(WebAutofillState::kPreviewed);
    WebInputElement lastname = GetInputElementById("lastname");
    lastname.SetAutofillState(WebAutofillState::kPreviewed);
    WebInputElement email = GetInputElementById("email");
    email.SetAutofillState(WebAutofillState::kPreviewed);
    WebInputElement email2 = GetInputElementById("email2");
    email2.SetAutofillState(WebAutofillState::kPreviewed);
    WebInputElement phone = GetInputElementById("phone");
    phone.SetAutofillState(WebAutofillState::kPreviewed);

    firstname.SetSuggestedValue(WebString::FromASCII("Wyatt"));
    lastname.SetSuggestedValue(WebString::FromASCII("Earp"));
    email.SetSuggestedValue(WebString::FromASCII("wyatt@earp.com"));
    email2.SetSuggestedValue(WebString::FromASCII("wyatt@earp.com"));
    phone.SetSuggestedValue(WebString::FromASCII("650-777-9999"));

    EXPECT_TRUE(
        ClearPreviewedFormWithElement(firstname, WebAutofillState::kNotFilled));

    EXPECT_EQ(ASCIIToUTF16("W"), firstname.Value().Utf16());
    EXPECT_TRUE(firstname.SuggestedValue().IsEmpty());
    EXPECT_FALSE(firstname.IsAutofilled());
    EXPECT_EQ(1, firstname.SelectionStart());
    EXPECT_EQ(1, firstname.SelectionEnd());

    EXPECT_TRUE(lastname.Value().IsEmpty());
    EXPECT_TRUE(lastname.SuggestedValue().IsEmpty());
    EXPECT_FALSE(lastname.IsAutofilled());
    EXPECT_TRUE(email.Value().IsEmpty());
    EXPECT_TRUE(email.SuggestedValue().IsEmpty());
    EXPECT_FALSE(email.IsAutofilled());
    EXPECT_TRUE(email2.Value().IsEmpty());
    EXPECT_TRUE(email2.SuggestedValue().IsEmpty());
    EXPECT_FALSE(email2.IsAutofilled());
    EXPECT_TRUE(phone.Value().IsEmpty());
    EXPECT_TRUE(phone.SuggestedValue().IsEmpty());
    EXPECT_FALSE(phone.IsAutofilled());
  }

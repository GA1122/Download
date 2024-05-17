  void ValidateFilledField(const AutofillFieldCase& field_case,
                           GetValueFunction get_value_function) {
    SCOPED_TRACE(base::StringPrintf("Verify autofilled value for field %s",
                                    field_case.id_attribute));
    WebString value;
    WebFormControlElement element = GetFormControlElementById(
        WebString::FromASCII(field_case.id_attribute));
    if ((element.FormControlType() == "select-one") ||
        (element.FormControlType() == "textarea")) {
      value = get_value_function(element);
    } else {
      ASSERT_TRUE(element.FormControlType() == "text" ||
                  element.FormControlType() == "month");
      value = get_value_function(element);
    }

    const WebString expected_value =
        WebString::FromASCII(field_case.expected_value);
    if (expected_value.IsEmpty())
      EXPECT_TRUE(value.IsEmpty());
    else
      EXPECT_EQ(expected_value.Utf8(), value.Utf8());

    EXPECT_EQ(field_case.should_be_autofilled, element.IsAutofilled());
  }

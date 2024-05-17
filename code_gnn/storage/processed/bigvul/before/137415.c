void RenderViewTest::SimulateUserInputChangeForElement(
    blink::WebInputElement* input,
    const std::string& new_value) {
  ASSERT_TRUE(base::IsStringASCII(new_value));
  while (!input->Focused())
    input->GetDocument().GetFrame()->View()->AdvanceFocus(false);

  size_t previous_length = input->Value().length();
  for (size_t i = 0; i < previous_length; ++i)
    SimulateUserTypingASCIICharacter(ui::VKEY_BACK, false);

  EXPECT_TRUE(input->Value().Utf8().empty());
  for (size_t i = 0; i < new_value.size(); ++i)
    SimulateUserTypingASCIICharacter(new_value[i], false);

  EXPECT_EQ(new_value, input->Value().Utf8().substr(0, new_value.length()));

  base::RunLoop().RunUntilIdle();
}

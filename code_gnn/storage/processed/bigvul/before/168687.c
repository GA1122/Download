std::unique_ptr<TextInput> UiSceneCreator::CreateTextInput(
    float font_height_meters,
    Model* model,
    TextInputInfo* text_input_model,
    TextInputDelegate* text_input_delegate) {
  auto text_input = base::MakeUnique<TextInput>(
      font_height_meters,
      base::BindRepeating(
          [](Model* model, bool focused) { model->editing_input = focused; },
          base::Unretained(model)),
      base::BindRepeating(
          [](TextInputInfo* model, const TextInputInfo& text_input_info) {
            *model = text_input_info;
          },
          base::Unretained(text_input_model)));
  text_input->SetDrawPhase(kPhaseNone);
  text_input->set_hit_testable(false);
  text_input->SetTextInputDelegate(text_input_delegate);
  text_input->AddBinding(base::MakeUnique<Binding<TextInputInfo>>(
      base::BindRepeating([](TextInputInfo* info) { return *info; },
                          base::Unretained(text_input_model)),
      base::BindRepeating(
          [](TextInput* e, const TextInputInfo& value) {
            e->UpdateInput(value);
          },
          base::Unretained(text_input.get()))));
  return text_input;
}

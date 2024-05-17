  void SetHasCompositionTextToTrue() {
    ui::CompositionText composition_text;
    composition_text.text = base::ASCIIToUTF16("text");
    tab_view()->SetCompositionText(composition_text);
    EXPECT_TRUE(has_composition_text());
  }

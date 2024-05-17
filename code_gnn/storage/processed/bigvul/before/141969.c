  MockAutofillPopupController() {
    gfx::FontList::SetDefaultFontDescription("Arial, Times New Roman, 15px");
    layout_model_.reset(
        new AutofillPopupLayoutModel(this, false  ));
  }

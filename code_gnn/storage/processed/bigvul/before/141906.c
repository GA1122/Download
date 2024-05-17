void AutofillPopupViewNativeViews::CreateChildViews() {
  RemoveAllChildViews(true  );
  rows_.clear();

  int line_number = 0;
  bool has_footer = false;

  while (line_number < controller_->GetLineCount()) {
    int frontend_id = controller_->GetSuggestionAt(line_number).frontend_id;
    switch (frontend_id) {
      case autofill::PopupItemId::POPUP_ITEM_ID_CLEAR_FORM:
      case autofill::PopupItemId::POPUP_ITEM_ID_AUTOFILL_OPTIONS:
      case autofill::PopupItemId::POPUP_ITEM_ID_SCAN_CREDIT_CARD:
      case autofill::PopupItemId::POPUP_ITEM_ID_CREDIT_CARD_SIGNIN_PROMO:
      case autofill::PopupItemId::POPUP_ITEM_ID_ALL_SAVED_PASSWORDS_ENTRY:
      case autofill::PopupItemId::POPUP_ITEM_ID_SHOW_ACCOUNT_CARDS:
        has_footer = true;
        break;

      case autofill::PopupItemId::POPUP_ITEM_ID_SEPARATOR:
        rows_.push_back(AutofillPopupSeparatorView::Create(this, line_number));
        break;

      case autofill::PopupItemId::
          POPUP_ITEM_ID_INSECURE_CONTEXT_PAYMENT_DISABLED_MESSAGE:
        rows_.push_back(AutofillPopupWarningView::Create(this, line_number));
        break;

      case autofill::PopupItemId::POPUP_ITEM_ID_USERNAME_ENTRY:
      case autofill::PopupItemId::POPUP_ITEM_ID_PASSWORD_ENTRY:
        rows_.push_back(PasswordPopupSuggestionView::Create(this, line_number,
                                                            frontend_id));
        break;

      default:
        rows_.push_back(AutofillPopupSuggestionView::Create(this, line_number,
                                                            frontend_id));
    }

    if (has_footer)
      break;
    line_number++;
  }

  if (rows_.size()) {
    views::View* body_container = new views::View();
    views::BoxLayout* body_layout = body_container->SetLayoutManager(
        std::make_unique<views::BoxLayout>(views::BoxLayout::kVertical));
    body_layout->set_main_axis_alignment(
        views::BoxLayout::MAIN_AXIS_ALIGNMENT_START);
    for (auto* row : rows_) {
      body_container->AddChildView(row);
    }

    scroll_view_ = new views::ScrollView();
    scroll_view_->set_hide_horizontal_scrollbar(true);
    scroll_view_->SetContents(body_container);
    scroll_view_->set_draw_overflow_indicator(false);
    scroll_view_->ClipHeightTo(0, body_container->GetPreferredSize().height());

    views::View* padding_wrapper = new views::View();
    padding_wrapper->SetBorder(
        views::CreateEmptyBorder(gfx::Insets(GetContentsVerticalPadding(), 0)));
    padding_wrapper->SetLayoutManager(std::make_unique<views::FillLayout>());
    padding_wrapper->AddChildView(scroll_view_);
    AddChildView(padding_wrapper);
    layout_->SetFlexForView(padding_wrapper, 1);
  }

  if (has_footer) {
    views::View* footer_container = new views::View();
    footer_container->SetBackground(
        views::CreateSolidBackground(GetFooterBackgroundColor()));

    views::BoxLayout* footer_layout = footer_container->SetLayoutManager(
        std::make_unique<views::BoxLayout>(views::BoxLayout::kVertical));
    footer_layout->set_main_axis_alignment(
        views::BoxLayout::MAIN_AXIS_ALIGNMENT_START);

    while (line_number < controller_->GetLineCount()) {
      rows_.push_back(AutofillPopupFooterView::Create(
          this, line_number,
          controller_->GetSuggestionAt(line_number).frontend_id));
      footer_container->AddChildView(rows_.back());
      line_number++;
    }

    AddChildView(footer_container);
    layout_->SetFlexForView(footer_container, 0);
  }
}

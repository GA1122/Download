views::View* ProfileChooserView::CreateSupervisedUserDisclaimerView() {
  views::View* view = new views::View();
  int horizontal_margin = kMenuEdgeMargin;
  views::GridLayout* layout =
      CreateSingleColumnLayout(view, menu_width_ - 2 * horizontal_margin);
  view->SetBorder(views::CreateEmptyBorder(0, horizontal_margin,
                                           kMenuEdgeMargin, horizontal_margin));

  views::Label* disclaimer = new views::Label(
      avatar_menu_->GetSupervisedUserInformation(), CONTEXT_BODY_TEXT_SMALL);
  disclaimer->SetMultiLine(true);
  disclaimer->SetAllowCharacterBreak(true);
  disclaimer->SetHorizontalAlignment(gfx::ALIGN_LEFT);
  layout->StartRow(1.0, 0);
  layout->AddView(disclaimer);

  return view;
}

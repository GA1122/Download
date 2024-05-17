views::View* PageInfoBubbleView::CreateSiteSettingsView(int side_margin) {
  views::View* site_settings_view = new views::View();
  views::BoxLayout* box_layout = new views::BoxLayout(
      views::BoxLayout::kVertical, gfx::Insets(0, side_margin),
      ChromeLayoutProvider::Get()->GetDistanceMetric(
          DISTANCE_CONTROL_LIST_VERTICAL));
  site_settings_view->SetLayoutManager(box_layout);
  box_layout->set_cross_axis_alignment(
      views::BoxLayout::CROSS_AXIS_ALIGNMENT_STRETCH);

  return site_settings_view;
}

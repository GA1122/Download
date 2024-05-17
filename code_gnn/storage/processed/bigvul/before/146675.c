PageInfoBubbleView::PageInfoBubbleView(
    views::View* anchor_view,
    const gfx::Rect& anchor_rect,
    gfx::NativeView parent_window,
    Profile* profile,
    content::WebContents* web_contents,
    const GURL& url,
    const security_state::SecurityInfo& security_info)
    : content::WebContentsObserver(web_contents),
      BubbleDialogDelegateView(anchor_view, views::BubbleBorder::TOP_LEFT),
      profile_(profile),
      header_(nullptr),
      site_settings_view_(nullptr),
      cookie_dialog_link_(nullptr),
      weak_factory_(this) {
  g_shown_bubble_type = BUBBLE_PAGE_INFO;
  g_page_info_bubble = this;
  set_parent_window(parent_window);
  if (!anchor_view)
    SetAnchorRect(anchor_rect);

  set_anchor_view_insets(gfx::Insets(
      GetLayoutConstant(LOCATION_BAR_BUBBLE_ANCHOR_VERTICAL_INSET), 0));

  const int side_margin = margins().left();
  DCHECK_EQ(margins().left(), margins().right());
  set_margins(gfx::Insets(margins().top(), 0, margins().bottom(), 0));

  GridLayout* layout = GridLayout::CreateAndInstall(this);
  constexpr int kColumnId = 0;
  views::ColumnSet* column_set = layout->AddColumnSet(kColumnId);
  column_set->AddColumn(GridLayout::FILL, GridLayout::FILL, 1,
                        GridLayout::USE_PREF, 0, 0);

  layout->StartRow(0, kColumnId);
  header_ = new BubbleHeaderView(this, this, side_margin);
  layout->AddView(header_);

  layout->StartRow(0, kColumnId);
  permissions_view_ = new views::View;
  layout->AddView(permissions_view_);

  layout->StartRow(0, kColumnId);
  layout->AddView(new views::Separator());

  ChromeLayoutProvider* layout_provider = ChromeLayoutProvider::Get();
  const int vertical_spacing = layout_provider->GetDistanceMetric(
      views::DISTANCE_UNRELATED_CONTROL_VERTICAL);
  layout->StartRowWithPadding(0, kColumnId, 0, vertical_spacing);
  site_settings_view_ = CreateSiteSettingsView(side_margin);
  layout->AddView(site_settings_view_);

  layout->StartRowWithPadding(0, kColumnId, 0, vertical_spacing);
  layout->AddView(CreateSiteSettingsLink(side_margin, this));

  if (!ui::MaterialDesignController::IsSecondaryUiMaterial()) {
    set_title_margins(gfx::Insets(
        layout_provider->GetInsetsMetric(views::INSETS_DIALOG).top(),
        side_margin, 0, side_margin));
  }
  views::BubbleDialogDelegateView::CreateBubble(this);

  presenter_.reset(new PageInfo(
      this, profile, TabSpecificContentSettings::FromWebContents(web_contents),
      web_contents, url, security_info));
}

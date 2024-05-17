FolderHeaderView::FolderHeaderView(FolderHeaderViewDelegate* delegate)
    : folder_item_(NULL),
      back_button_(new views::ImageButton(this)),
      folder_name_view_(new FolderNameView),
      folder_name_placeholder_text_(
          ui::ResourceBundle::GetSharedInstance().GetLocalizedString(
              IDS_APP_LIST_FOLDER_NAME_PLACEHOLDER)),
      delegate_(delegate),
      folder_name_visible_(true) {
  ui::ResourceBundle& rb = ui::ResourceBundle::GetSharedInstance();
  back_button_->SetImage(views::ImageButton::STATE_NORMAL,
      rb.GetImageSkiaNamed(IDR_APP_LIST_FOLDER_BACK_NORMAL));
  back_button_->SetImageAlignment(views::ImageButton::ALIGN_CENTER,
      views::ImageButton::ALIGN_MIDDLE);
  AddChildView(back_button_);

  folder_name_view_->SetFontList(
      rb.GetFontList(ui::ResourceBundle::MediumFont));
  folder_name_view_->set_placeholder_text_color(kHintTextColor);
  folder_name_view_->set_placeholder_text(folder_name_placeholder_text_);
  folder_name_view_->SetBorder(views::Border::NullBorder());
  folder_name_view_->SetBackgroundColor(kContentsBackgroundColor);
  folder_name_view_->set_controller(this);
  AddChildView(folder_name_view_);
}

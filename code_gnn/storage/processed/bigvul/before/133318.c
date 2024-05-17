 views::Separator* CreateSeparator(views::Separator::Orientation orientation) {
   const int kSeparatorInset = 10;

  views::Separator* separator =
      new views::Separator(views::Separator::HORIZONTAL);
  separator->SetColor(ash::kBorderDarkColor);
  separator->SetBorder(
      views::Border::CreateEmptyBorder(kSeparatorInset, 0, kSeparatorInset, 0));
  return separator;
}

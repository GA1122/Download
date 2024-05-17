  FolderNameView() {
    SetBorder(views::Border::CreateEmptyBorder(1, 1, 1, 1));
    const SkColor kFocusBorderColor = SkColorSetRGB(64, 128, 250);
    SetFocusPainter(views::Painter::CreateSolidFocusPainter(
          kFocusBorderColor,
          gfx::Insets(0, 0, 1, 1)));
  }

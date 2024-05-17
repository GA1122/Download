gfx::ImageSkia GetFolderIcon(gfx::VectorIconId id, SkColor text_color) {
  return gfx::CreateVectorIcon(id,
                               color_utils::DeriveDefaultIconColor(text_color));
}

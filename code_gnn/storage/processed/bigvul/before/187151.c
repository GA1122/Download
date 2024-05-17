 gfx::Insets ScrollableShelfView::CalculateEdgePadding() const {
   if (ShouldApplyDisplayCentering())
     return CalculatePaddingForDisplayCentering();
  
    const int icons_size = shelf_view_->GetSizeOfAppIcons(
        shelf_view_->number_of_visible_apps(), false);
  gfx::Insets padding_insets(
        0,
        ShelfConfig::Get()->app_icon_group_margin());
//   const int base_padding = ShelfConfig::Get()->app_icon_group_margin();
  
    const int available_size_for_app_icons =
        (GetShelf()->IsHorizontalAlignment() ? width() : height()) -
       2 * ShelfConfig::Get()->app_icon_group_margin();
 
   int gap =
       layout_strategy_ == kNotShowArrowButtons
            ? available_size_for_app_icons - icons_size   
            : CalculateOverflowPadding(available_size_for_app_icons);   
  
  padding_insets.set_left(padding_insets.left() + gap / 2);
  padding_insets.set_right(padding_insets.right() +
                           (gap % 2 ? gap / 2 + 1 : gap / 2));
//    
//   const int before_padding = base_padding + gap / 2;
//   const int after_padding = base_padding + (gap % 2 ? gap / 2 + 1 : gap / 2);
// 
//   gfx::Insets padding_insets;
//   if (GetShelf()->IsHorizontalAlignment()) {
//     padding_insets =
//         gfx::Insets( 0, before_padding,  0, after_padding);
//   } else {
//     padding_insets =
//         gfx::Insets(before_padding,  0, after_padding,  0);
//   }
  
    return padding_insets;
  }
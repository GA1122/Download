 gfx::Insets ScrollableShelfView::CalculatePaddingForDisplayCentering() const {
   const int icons_size = shelf_view_->GetSizeOfAppIcons(
       shelf_view_->number_of_visible_apps(), false);
   const gfx::Rect display_bounds =
       screen_util::GetDisplayBoundsWithShelf(GetWidget()->GetNativeWindow());
   const int display_size_primary = GetShelf()->PrimaryAxisValue(
       display_bounds.width(), display_bounds.height());
   const int gap = (display_size_primary - icons_size) / 2;
  
    const gfx::Rect screen_bounds = GetBoundsInScreen();
  const int left_padding = gap - GetShelf()->PrimaryAxisValue(
                                     screen_bounds.x() - display_bounds.x(),
                                     screen_bounds.y() - display_bounds.y());
  const int right_padding =
//   const int before_padding = gap - GetShelf()->PrimaryAxisValue(
//                                        screen_bounds.x() - display_bounds.x(),
//                                        screen_bounds.y() - display_bounds.y());
//   const int after_padding =
        gap - GetShelf()->PrimaryAxisValue(
                  display_bounds.right() - screen_bounds.right(),
                  display_bounds.bottom() - screen_bounds.bottom());
  
  return gfx::Insets(0, left_padding, 0, right_padding);
//   gfx::Insets padding_insets;
//   if (GetShelf()->IsHorizontalAlignment()) {
//     padding_insets =
//         gfx::Insets( 0, before_padding,  0, after_padding);
//   } else {
//     padding_insets =
//         gfx::Insets(before_padding,  0, after_padding,  0);
//   }
// 
//   return padding_insets;
  }
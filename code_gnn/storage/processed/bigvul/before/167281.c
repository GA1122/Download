int CalculateOverflowPadding(int available_size) {
  return (available_size - kArrowButtonGroupWidth -
          ShelfConfig::Get()->button_size() - GetAppIconEndPadding()) %
         GetUnit();
}

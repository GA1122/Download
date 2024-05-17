int GetUnit() {
  return ShelfConfig::Get()->button_size() +
         ShelfConfig::Get()->button_spacing();
}

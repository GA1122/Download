bool DoTitlesDiffer(const MostVisitedURLList& old_list,
                    const MostVisitedURLList& new_list) {
  if (old_list.size() != new_list.size())
    return true;

  return !std::equal(std::begin(old_list), std::end(old_list),
                     std::begin(new_list),
                     [](const auto& old_item_ptr, const auto& new_item_ptr) {
                       return old_item_ptr.title == new_item_ptr.title;
                     });
}

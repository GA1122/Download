  views::Button* GetButtonForAction(MediaSessionAction action) const {
    const auto& children = button_row()->children();
    const auto it = std::find_if(
        children.begin(), children.end(), [action](const views::View* v) {
          return views::Button::AsButton(v)->tag() == static_cast<int>(action);
        });

    if (it == children.end())
      return nullptr;

    return views::Button::AsButton(*it);
  }

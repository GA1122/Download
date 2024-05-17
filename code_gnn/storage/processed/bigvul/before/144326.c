void LoginDisplayHostWebUI::OnWillRemoveView(views::Widget* widget,
                                             views::View* view) {
  if (view != static_cast<views::View*>(login_view_))
    return;
  login_view_ = nullptr;
  widget->RemoveRemovalsObserver(this);
}

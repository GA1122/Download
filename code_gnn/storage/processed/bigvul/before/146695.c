void PerformMouseClickOnView(views::View* view) {
  ui::AXActionData data;
  data.action = ui::AX_ACTION_DO_DEFAULT;
  view->HandleAccessibleAction(data);
}

 CardUnmaskPromptView* CardUnmaskPromptView::CreateAndShow(
     CardUnmaskPromptController* controller) {
  CardUnmaskPromptViews* view = new CardUnmaskPromptViews(controller);
  view->Show();
  return view;
}

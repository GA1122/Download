AutofillPopupView* AutofillPopupView::Create(
    AutofillPopupController* controller) {
#if defined(OS_MACOSX)
  if (!platform_util::GetTopLevel(controller->container_view()))
    return nullptr;
#endif

  views::Widget* observing_widget =
      views::Widget::GetTopLevelWidgetForNativeView(
          controller->container_view());

#if !defined(OS_MACOSX)
  if (!observing_widget)
    return nullptr;
#endif

  if (features::ShouldUseNativeViews())
    return new AutofillPopupViewNativeViews(controller, observing_widget);

  return new AutofillPopupViewViews(controller, observing_widget);
}

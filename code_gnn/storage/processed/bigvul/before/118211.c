AutofillDialogViews::DetailsContainerView::DetailsContainerView(
    const base::Closure& callback)
    : bounds_changed_callback_(callback),
      ignore_layouts_(false) {}

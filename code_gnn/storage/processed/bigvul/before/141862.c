AutofillPopupBaseView::AutofillPopupBaseView(
    AutofillPopupViewDelegate* delegate,
    views::Widget* parent_widget)
    : delegate_(delegate),
      parent_widget_(parent_widget),
      weak_ptr_factory_(this) {}

  AutofillPopupItemView(AutofillPopupViewNativeViews* popup_view,
                        int line_number,
                        int frontend_id,
                        int extra_height = 0)
      : AutofillPopupRowView(popup_view, line_number),
        frontend_id_(frontend_id),
        extra_height_(extra_height) {}

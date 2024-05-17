  NotificationView(const DialogNotification& data,
                   AutofillDialogViewDelegate* delegate)
      : data_(data),
        delegate_(delegate),
        checkbox_(NULL) {
    scoped_ptr<views::View> label_view;
    if (data.HasCheckbox()) {
      scoped_ptr<views::Checkbox> checkbox(
          new views::Checkbox(base::string16()));
      checkbox->SetText(data.display_text());
      checkbox->SetTextMultiLine(true);
      checkbox->SetHorizontalAlignment(gfx::ALIGN_LEFT);
      checkbox->SetTextColor(views::Button::STATE_NORMAL,
                             data.GetTextColor());
      checkbox->SetTextColor(views::Button::STATE_HOVERED,
                             data.GetTextColor());
      checkbox->SetChecked(data.checked());
      checkbox->set_listener(this);
      checkbox_ = checkbox.get();
      label_view.reset(checkbox.release());
    } else {
      scoped_ptr<views::StyledLabel> label(new views::StyledLabel(
          data.display_text(), this));
      label->set_auto_color_readability_enabled(false);

      views::StyledLabel::RangeStyleInfo text_style;
      text_style.color = data.GetTextColor();

      if (data.link_range().is_empty()) {
        label->AddStyleRange(gfx::Range(0, data.display_text().size()),
                             text_style);
      } else {
        gfx::Range prefix_range(0, data.link_range().start());
        if (!prefix_range.is_empty())
          label->AddStyleRange(prefix_range, text_style);

        label->AddStyleRange(
            data.link_range(),
            views::StyledLabel::RangeStyleInfo::CreateForLink());

        gfx::Range suffix_range(data.link_range().end(),
                                data.display_text().size());
        if (!suffix_range.is_empty())
          label->AddStyleRange(suffix_range, text_style);
      }

      label_view.reset(label.release());
    }

    AddChildView(label_view.release());

    if (!data.tooltip_text().empty())
      AddChildView(new TooltipIcon(data.tooltip_text()));

    set_background(
       views::Background::CreateSolidBackground(data.GetBackgroundColor()));
    SetBorder(views::Border::CreateSolidSidedBorder(
        1, 0, 1, 0, data.GetBorderColor()));
  }

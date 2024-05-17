void AutofillDialogViews::InitInputsView(DialogSection section) {
  DetailsGroup* group = GroupForSection(section);
  EraseInvalidViewsInGroup(group);

  TextfieldMap* textfields = &group->textfields;
  textfields->clear();

  ComboboxMap* comboboxes = &group->comboboxes;
  comboboxes->clear();

  views::View* view = group->manual_input;
  view->RemoveAllChildViews(true);

  views::GridLayout* layout = new views::GridLayout(view);
  view->SetLayoutManager(layout);

  int column_set_id = 0;
  const DetailInputs& inputs = delegate_->RequestedFieldsForSection(section);
  for (DetailInputs::const_iterator it = inputs.begin();
       it != inputs.end(); ++it) {
    const DetailInput& input = *it;

    ui::ComboboxModel* input_model =
        delegate_->ComboboxModelForAutofillType(input.type);
    scoped_ptr<views::View> view_to_add;
    if (input_model) {
      views::Combobox* combobox = new views::Combobox(input_model);
      combobox->set_listener(this);
      comboboxes->insert(std::make_pair(input.type, combobox));
      SelectComboboxValueOrSetToDefault(combobox, input.initial_value);
      view_to_add.reset(combobox);
    } else {
      ExpandingTextfield* field = new ExpandingTextfield(input.initial_value,
                                                         input.placeholder_text,
                                                         input.IsMultiline(),
                                                         this);
      textfields->insert(std::make_pair(input.type, field));
      view_to_add.reset(field);
    }

    if (input.length == DetailInput::NONE) {
      other_owned_views_.push_back(view_to_add.release());
      continue;
    }

    if (input.length == DetailInput::LONG)
      ++column_set_id;

    views::ColumnSet* column_set = layout->GetColumnSet(column_set_id);
    if (!column_set) {
      column_set = layout->AddColumnSet(column_set_id);
      if (it != inputs.begin())
        layout->AddPaddingRow(0, kManualInputRowPadding);
      layout->StartRow(0, column_set_id);
    } else {
      column_set->AddPaddingColumn(0, views::kRelatedControlHorizontalSpacing);
      layout->SkipColumns(1);
    }

    float expand = input.expand_weight;
    column_set->AddColumn(views::GridLayout::FILL,
                          views::GridLayout::FILL,
                          expand ? expand : 1.0,
                          views::GridLayout::USE_PREF,
                          0,
                          0);

    layout->AddView(view_to_add.release(), 1, 1,
                    views::GridLayout::FILL, views::GridLayout::FILL,
                    1, 0);

    if (input.length == DetailInput::LONG ||
        input.length == DetailInput::SHORT_EOL) {
      ++column_set_id;
    }
  }

  SetIconsForSection(section);
}

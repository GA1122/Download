void SelectComboboxValueOrSetToDefault(views::Combobox* combobox,
                                       const base::string16& value) {
  if (!combobox->SelectValue(value))
    combobox->SetSelectedIndex(combobox->model()->GetDefaultIndex());
}

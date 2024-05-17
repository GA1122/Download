base::string16 PageInfoUI::ChosenObjectToUIString(
    const ChosenObjectInfo& object) {
  return base::UTF8ToUTF16(
      object.ui_info.get_object_name(object.chooser_object->value));
}

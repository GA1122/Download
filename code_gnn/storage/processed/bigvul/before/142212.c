    static void RegisterJSONConverter(
        base::JSONValueConverter<EntryCapabilities>* converter) {
      converter->RegisterBoolField("canCopy", &EntryCapabilities::can_copy);
      converter->RegisterBoolField("canDelete", &EntryCapabilities::can_delete);
      converter->RegisterBoolField("canRename", &EntryCapabilities::can_rename);
      converter->RegisterBoolField("canAddChildren",
                                   &EntryCapabilities::can_add_children);
      converter->RegisterBoolField("canShare", &EntryCapabilities::can_share);
    }

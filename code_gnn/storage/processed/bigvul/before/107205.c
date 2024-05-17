  void SetFieldTypes(const std::vector<AutoFillFieldType>& heuristic_types,
                     const std::vector<AutoFillFieldType>& server_types) {
    ASSERT_EQ(field_count(), heuristic_types.size());
    ASSERT_EQ(field_count(), server_types.size());

    for (size_t i = 0; i < field_count(); ++i) {
      AutoFillField* field = (*fields())[i];
      ASSERT_TRUE(field);
      field->set_heuristic_type(heuristic_types[i]);
      field->set_server_type(server_types[i]);
    }

     UpdateAutoFillCount();
   }

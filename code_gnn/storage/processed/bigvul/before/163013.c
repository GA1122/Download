void ValidateDump(base::Value* heaps_v2,
                  int expected_alloc_size,
                  int expected_alloc_count,
                  const char* allocator_name,
                  const char* type_name) {
  base::Value* sizes =
      heaps_v2->FindPath({"allocators", allocator_name, "sizes"});
  ASSERT_TRUE(sizes);
  const base::Value::ListStorage& sizes_list = sizes->GetList();
  EXPECT_FALSE(sizes_list.empty());

  base::Value* counts =
      heaps_v2->FindPath({"allocators", allocator_name, "counts"});
  ASSERT_TRUE(counts);
  const base::Value::ListStorage& counts_list = counts->GetList();
  EXPECT_EQ(sizes_list.size(), counts_list.size());

  base::Value* types =
      heaps_v2->FindPath({"allocators", allocator_name, "types"});
  ASSERT_TRUE(types);
  const base::Value::ListStorage& types_list = types->GetList();
  EXPECT_FALSE(types_list.empty());
  EXPECT_EQ(sizes_list.size(), types_list.size());

  bool found_browser_alloc = false;
  size_t browser_alloc_index = 0;
  for (size_t i = 0; i < sizes_list.size(); i++) {
    if (counts_list[i].GetInt() == expected_alloc_count &&
        sizes_list[i].GetInt() != expected_alloc_size) {
      LOG(WARNING) << "Allocation candidate (size:" << sizes_list[i].GetInt()
                   << " count:" << counts_list[i].GetInt() << ")";
    }
    if (sizes_list[i].GetInt() == expected_alloc_size &&
        counts_list[i].GetInt() == expected_alloc_count) {
      browser_alloc_index = i;
      found_browser_alloc = true;
      break;
    }
  }

  ASSERT_TRUE(found_browser_alloc)
      << "Failed to find an allocation of the "
         "appropriate size. Did the send buffer "
         "not flush? (size: "
      << expected_alloc_size << " count:" << expected_alloc_count << ")";

  if (type_name) {
    bool found = false;
    int type = types_list[browser_alloc_index].GetInt();
    base::Value* strings = heaps_v2->FindPath({"maps", "strings"});
    for (base::Value& dict : strings->GetList()) {
      int id = dict.FindKey("id")->GetInt();
      if (id == type) {
        found = true;
        std::string name = dict.FindKey("string")->GetString();
        EXPECT_STREQ(name.c_str(), type_name);
        break;
      }
    }
    EXPECT_TRUE(found) << "Failed to find type name string.";
  }
}

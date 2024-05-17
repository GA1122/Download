   void CreateTestOptimizationHints(const std::string& hints_content) {
     base::FilePath hints_path = component_install_dir().Append(
         optimization_guide::kUnindexedHintsFileName);
    ASSERT_EQ(static_cast<int32_t>(hints_content.length()),
              base::WriteFile(hints_path, hints_content.data(),
                              hints_content.length()));
  }

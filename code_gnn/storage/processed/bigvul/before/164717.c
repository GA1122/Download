   void ExpectLabels(const char* html,
                     const std::vector<base::string16>& id_attributes,
                     const std::vector<base::string16>& name_attributes,
                    const std::vector<base::string16>& labels,
                    const std::vector<base::string16>& names,
                    const std::vector<base::string16>& values) {
    std::vector<std::string> control_types(labels.size(), "text");
    ExpectLabelsAndTypes(html, id_attributes, name_attributes, labels, names,
                         values, control_types);
  }

  std::string GetIndicesClosedByCommandAsString(
      const TabStripModel& model,
      int index,
      TabStripModel::ContextMenuCommand id) const {
    std::vector<int> indices = model.GetIndicesClosedByCommand(index, id);
    std::string result;
    for (size_t i = 0; i < indices.size(); ++i) {
      if (i != 0)
        result += " ";
      result += base::IntToString(indices[i]);
    }
    return result;
  }

bool TabStripModel::OpenerMatches(const WebContentsData* data,
                                  const WebContents* opener,
                                  bool use_group) {
  return data->opener == opener || (use_group && data->group == opener);
}

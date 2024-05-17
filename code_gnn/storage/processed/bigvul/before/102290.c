static FilePath::StringType MakePathRelative(const FilePath& parent,
                                             const FilePath& child) {
  if (!parent.IsParent(child))
    return child.value();

  FilePath::StringType retval = child.value().substr(
      parent.value().length());
  if (FilePath::IsSeparator(retval[0]))
    return retval.substr(1);
  else
    return retval;
}

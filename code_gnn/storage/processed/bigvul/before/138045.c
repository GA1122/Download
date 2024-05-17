static HashSet<QualifiedName>& getLandmarkRolesNotAllowed() {
  DEFINE_STATIC_LOCAL(HashSet<QualifiedName>, landmarkRolesNotAllowed, ());
  if (landmarkRolesNotAllowed.isEmpty()) {
    landmarkRolesNotAllowed.insert(articleTag);
    landmarkRolesNotAllowed.insert(asideTag);
    landmarkRolesNotAllowed.insert(navTag);
    landmarkRolesNotAllowed.insert(sectionTag);
    landmarkRolesNotAllowed.insert(blockquoteTag);
    landmarkRolesNotAllowed.insert(detailsTag);
    landmarkRolesNotAllowed.insert(fieldsetTag);
    landmarkRolesNotAllowed.insert(figureTag);
    landmarkRolesNotAllowed.insert(tdTag);
    landmarkRolesNotAllowed.insert(mainTag);
  }
  return landmarkRolesNotAllowed;
}

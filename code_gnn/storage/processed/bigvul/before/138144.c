static ARIARoleMap* createARIARoleMap() {
  ARIARoleMap* roleMap = new ARIARoleMap;

  for (size_t i = 0; i < WTF_ARRAY_LENGTH(roles); ++i)
    roleMap->set(String(roles[i].ariaRole), roles[i].webcoreRole);
  return roleMap;
}

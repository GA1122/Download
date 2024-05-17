static Vector<AtomicString>* createInternalRoleNameVector() {
  Vector<AtomicString>* internalRoleNameVector =
      new Vector<AtomicString>(NumRoles);
  for (size_t i = 0; i < WTF_ARRAY_LENGTH(internalRoles); i++)
    (*internalRoleNameVector)[internalRoles[i].webcoreRole] =
        AtomicString(internalRoles[i].internalRoleName);

  return internalRoleNameVector;
}

  void DeleteSusanInRoot() {
    ScopedDirLookup dir(syncdb_.manager(), syncdb_.name());
    ASSERT_TRUE(dir.good());

    const syncable::Id susan_id = TestIdFactory::FromNumber(susan_int_id_);
    ASSERT_GT(countdown_till_delete_, 0);
    if (0 != --countdown_till_delete_)
    return;
  WriteTransaction trans(FROM_HERE, UNITTEST, dir);
    MutableEntry susan(&trans, GET_BY_ID, susan_id);
  Directory::ChildHandles children;
  dir->GetChildHandlesById(&trans, susan.Get(ID), &children);
  ASSERT_TRUE(0 == children.size());
  susan.Put(IS_DEL, true);
  susan.Put(IS_UNSYNCED, true);
}

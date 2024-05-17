  void MoveBobIntoID2() {
    ScopedDirLookup dir(syncdb_.manager(), syncdb_.name());
    CHECK(dir.good());

    WriteTransaction trans(FROM_HERE, UNITTEST, dir);
    Entry alice(&trans, GET_BY_ID,
                TestIdFactory::FromNumber(fred_id_number));
    CHECK(alice.good());
    EXPECT_TRUE(!alice.Get(IS_DEL));
    EXPECT_TRUE(alice.Get(SYNCING)) << "Expected to be called mid-commit.";
    MutableEntry bob(&trans, GET_BY_ID,
                     TestIdFactory::FromNumber(bob_id_number));
    CHECK(bob.good());
    bob.Put(IS_UNSYNCED, true);

    bob.Put(SYNCING, false);
    bob.Put(PARENT_ID, alice.Get(ID));
  }

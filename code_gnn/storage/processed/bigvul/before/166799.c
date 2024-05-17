void Copy() {
  Object* obj = Object::Create();
  Container container1;
  container1.insert(obj);
  {
    ExpectWriteBarrierFires scope(ThreadState::Current(), {obj});
    Container container2(container1);
    EXPECT_TRUE(container1.Contains(obj));
    EXPECT_TRUE(container2.Contains(obj));
  }
}

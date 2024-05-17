void InsertNoBarrier() {
  Object* obj = Object::Create();
  Container container;
  {
    ExpectNoWriteBarrierFires scope(ThreadState::Current(), {obj});
    container.insert(obj);
  }
}

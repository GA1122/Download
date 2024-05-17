void MoveNoBarrier() {
  Object* obj = Object::Create();
  Container container1;
  container1.insert(obj);
  {
    ExpectNoWriteBarrierFires scope(ThreadState::Current(), {obj});
    Container container2(std::move(container1));
  }
}

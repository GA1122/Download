void SwapNoBarrier() {
  Object* obj1 = Object::Create();
  Object* obj2 = Object::Create();
  Container container1;
  container1.insert(obj1);
  Container container2;
  container2.insert(obj2);
  {
    ExpectNoWriteBarrierFires scope(ThreadState::Current(), {obj1, obj2});
    std::swap(container1, container2);
  }
}

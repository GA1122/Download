static void spawn_reactor_thread(reactor_t *reactor) {
 int ret = pthread_create(&thread, NULL, reactor_thread, reactor);
  EXPECT_EQ(ret, 0);
}

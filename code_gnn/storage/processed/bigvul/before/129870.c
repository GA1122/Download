void TraceWithAllMacroVariants(WaitableEvent* task_complete_event) {
  {
    TRACE_EVENT0("all", "TRACE_EVENT0 call");
    TRACE_EVENT1("all", "TRACE_EVENT1 call", "name1", "value1");
    TRACE_EVENT2("all", "TRACE_EVENT2 call",
                 "name1", "\"value1\"",
                 "name2", "value\\2");

    TRACE_EVENT_INSTANT0("all", "TRACE_EVENT_INSTANT0 call",
                         TRACE_EVENT_SCOPE_GLOBAL);
    TRACE_EVENT_INSTANT1("all", "TRACE_EVENT_INSTANT1 call",
                         TRACE_EVENT_SCOPE_PROCESS, "name1", "value1");
    TRACE_EVENT_INSTANT2("all", "TRACE_EVENT_INSTANT2 call",
                         TRACE_EVENT_SCOPE_THREAD,
                         "name1", "value1",
                         "name2", "value2");

    TRACE_EVENT_BEGIN0("all", "TRACE_EVENT_BEGIN0 call");
    TRACE_EVENT_BEGIN1("all", "TRACE_EVENT_BEGIN1 call", "name1", "value1");
    TRACE_EVENT_BEGIN2("all", "TRACE_EVENT_BEGIN2 call",
                       "name1", "value1",
                       "name2", "value2");

    TRACE_EVENT_END0("all", "TRACE_EVENT_END0 call");
    TRACE_EVENT_END1("all", "TRACE_EVENT_END1 call", "name1", "value1");
    TRACE_EVENT_END2("all", "TRACE_EVENT_END2 call",
                     "name1", "value1",
                     "name2", "value2");

    TRACE_EVENT_ASYNC_BEGIN0("all", "TRACE_EVENT_ASYNC_BEGIN0 call", kAsyncId);
    TRACE_EVENT_ASYNC_BEGIN1("all", "TRACE_EVENT_ASYNC_BEGIN1 call", kAsyncId,
                             "name1", "value1");
    TRACE_EVENT_ASYNC_BEGIN2("all", "TRACE_EVENT_ASYNC_BEGIN2 call", kAsyncId,
                             "name1", "value1",
                             "name2", "value2");

    TRACE_EVENT_ASYNC_STEP_INTO0("all", "TRACE_EVENT_ASYNC_STEP_INTO0 call",
                                 kAsyncId, "step_begin1");
    TRACE_EVENT_ASYNC_STEP_INTO1("all", "TRACE_EVENT_ASYNC_STEP_INTO1 call",
                                 kAsyncId, "step_begin2", "name1", "value1");

    TRACE_EVENT_ASYNC_END0("all", "TRACE_EVENT_ASYNC_END0 call", kAsyncId);
    TRACE_EVENT_ASYNC_END1("all", "TRACE_EVENT_ASYNC_END1 call", kAsyncId,
                           "name1", "value1");
    TRACE_EVENT_ASYNC_END2("all", "TRACE_EVENT_ASYNC_END2 call", kAsyncId,
                           "name1", "value1",
                           "name2", "value2");

    TRACE_EVENT_FLOW_BEGIN0("all", "TRACE_EVENT_FLOW_BEGIN0 call", kFlowId);
    TRACE_EVENT_FLOW_STEP0("all", "TRACE_EVENT_FLOW_STEP0 call",
                           kFlowId, "step1");
    TRACE_EVENT_FLOW_END_BIND_TO_ENCLOSING0("all",
        "TRACE_EVENT_FLOW_END_BIND_TO_ENCLOSING0 call", kFlowId);

    TRACE_COUNTER1("all", "TRACE_COUNTER1 call", 31415);
    TRACE_COUNTER2("all", "TRACE_COUNTER2 call",
                   "a", 30000,
                   "b", 1415);

    TRACE_COUNTER_ID1("all", "TRACE_COUNTER_ID1 call", 0x319009, 31415);
    TRACE_COUNTER_ID2("all", "TRACE_COUNTER_ID2 call", 0x319009,
                      "a", 30000, "b", 1415);

    TRACE_EVENT_COPY_BEGIN_WITH_ID_TID_AND_TIMESTAMP0("all",
        "TRACE_EVENT_COPY_BEGIN_WITH_ID_TID_AND_TIMESTAMP0 call",
        kAsyncId, kThreadId, 12345);
    TRACE_EVENT_COPY_END_WITH_ID_TID_AND_TIMESTAMP0("all",
        "TRACE_EVENT_COPY_END_WITH_ID_TID_AND_TIMESTAMP0 call",
        kAsyncId, kThreadId, 23456);

    TRACE_EVENT_BEGIN_WITH_ID_TID_AND_TIMESTAMP0("all",
        "TRACE_EVENT_BEGIN_WITH_ID_TID_AND_TIMESTAMP0 call",
        kAsyncId2, kThreadId, 34567);
    TRACE_EVENT_ASYNC_STEP_PAST0("all", "TRACE_EVENT_ASYNC_STEP_PAST0 call",
                                 kAsyncId2, "step_end1");
    TRACE_EVENT_ASYNC_STEP_PAST1("all", "TRACE_EVENT_ASYNC_STEP_PAST1 call",
                                 kAsyncId2, "step_end2", "name1", "value1");

    TRACE_EVENT_END_WITH_ID_TID_AND_TIMESTAMP0("all",
        "TRACE_EVENT_END_WITH_ID_TID_AND_TIMESTAMP0 call",
        kAsyncId2, kThreadId, 45678);

    TRACE_EVENT_OBJECT_CREATED_WITH_ID("all", "tracked object 1", 0x42);
    TRACE_EVENT_OBJECT_SNAPSHOT_WITH_ID(
        "all", "tracked object 1", 0x42, "hello");
    TRACE_EVENT_OBJECT_DELETED_WITH_ID("all", "tracked object 1", 0x42);

    TraceScopedTrackableObject<int> trackable("all", "tracked object 2",
                                              0x2128506);
    trackable.snapshot("world");

    TRACE_EVENT1(kControlCharacters, kControlCharacters,
                 kControlCharacters, kControlCharacters);
  }   

  if (task_complete_event)
    task_complete_event->Signal();
}

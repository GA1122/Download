SyncTest::SyncTest(TestType test_type)
    : test_type_(test_type),
      server_type_(SERVER_TYPE_UNDECIDED),
      num_clients_(-1),
      use_verifier_(true),
      notifications_enabled_(true),
      test_server_handle_(base::kNullProcessHandle),
      number_of_default_sync_items_(0) {
  sync_datatype_helper::AssociateWithTest(this);
  switch (test_type_) {
    case SINGLE_CLIENT: {
      num_clients_ = 1;
      break;
    }
    case TWO_CLIENT: {
      num_clients_ = 2;
      break;
    }
    case MULTIPLE_CLIENT: {
      num_clients_ = 3;
      break;
    }
  }
}

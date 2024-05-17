  void WaitForAcceptedConnections(size_t num_connections) {
    DCHECK_CURRENTLY_ON(BrowserThread::UI);
    DCHECK(!num_accepted_connections_loop_);
    DCHECK_GT(num_connections, 0u);
    base::RunLoop run_loop;
    EXPECT_GE(num_connections, sockets_.size());
    num_accepted_connections_loop_ = &run_loop;
    num_accepted_connections_needed_ = num_connections;
    CheckAccepted();
    run_loop.Run();
    EXPECT_EQ(num_connections, sockets_.size());
  }

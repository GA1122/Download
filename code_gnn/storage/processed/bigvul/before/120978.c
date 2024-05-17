  void CompleteConnection(int result) {
    connection_callback_.Run(result);
  }

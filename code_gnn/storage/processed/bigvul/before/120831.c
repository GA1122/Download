  void ConnectionCallback(const BluetoothDevice *device,
                          scoped_refptr<BluetoothSocket> socket) {
    ++connection_callback_count_;
    last_device_ = device;
    last_socket_ = socket;

    message_loop_.Quit();
  }

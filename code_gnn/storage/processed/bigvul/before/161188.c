  void ResponseCallback(int index,
                        const MediaStreamDevices& devices,
                        std::unique_ptr<MediaStreamUIProxy> ui_proxy) {
    Response(index);
    base::ThreadTaskRunnerHandle::Get()->PostTask(FROM_HERE,
                                                  run_loop_.QuitClosure());
  }

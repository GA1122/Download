  void OnGenerateStream(int page_request_id,
                        const StreamControls& controls,
                        const base::Closure& quit_closure) {
    quit_closures_.push(quit_closure);
    MediaStreamDispatcherHost::GenerateStream(
        page_request_id, controls, false,
        base::BindOnce(&MockMediaStreamDispatcherHost::OnStreamGenerated,
                       base::Unretained(this), page_request_id));
  }

  base::RepeatingCallback<void()> CreateCallback() {
    callback_counter_++;
    return base::BindRepeating(&CallbackRunLoop::OnCallbackFired,
                               base::Unretained(this));
  }

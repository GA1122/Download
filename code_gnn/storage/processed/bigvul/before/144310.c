  LoginWidgetDelegate(views::Widget* widget, LoginDisplayHostWebUI* host)
      : widget_(widget), login_display_host_(host) {
    DCHECK(widget_);
    DCHECK(login_display_host_);
  }

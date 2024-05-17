    explicit WidgetActivatedWaiter(views::Widget* widget)
        : widget_(widget), waiting_(false) {
      widget_->AddObserver(this);
    }

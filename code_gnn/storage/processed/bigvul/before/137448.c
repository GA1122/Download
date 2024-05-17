MessageLoopForUI::MessageLoopForUI(std::unique_ptr<MessagePump> pump)
    : MessageLoop(TYPE_UI, BindOnce(&ReturnPump, std::move(pump))) {}

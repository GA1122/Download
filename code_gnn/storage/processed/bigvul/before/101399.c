  void RunGetNodesByIdFailureTest(const char* message_name) {
    StrictMock<MockJsReplyHandler> reply_handler;

    ListValue empty_list_args;
    empty_list_args.Append(new ListValue());

    EXPECT_CALL(reply_handler,
                HandleJsReply(message_name,
                                    HasArgsAsList(empty_list_args)))
        .Times(6);

    {
      ListValue args;
      SendJsMessage(message_name,
                    JsArgList(&args), reply_handler.AsWeakHandle());
    }

    {
      ListValue args;
      args.Append(new ListValue());
      SendJsMessage(message_name,
                    JsArgList(&args), reply_handler.AsWeakHandle());
    }

    {
      ListValue args;
      ListValue* ids = new ListValue();
      args.Append(ids);
      ids->Append(Value::CreateStringValue(""));
      SendJsMessage(message_name,
                    JsArgList(&args), reply_handler.AsWeakHandle());
    }

    {
      ListValue args;
      ListValue* ids = new ListValue();
      args.Append(ids);
      ids->Append(Value::CreateStringValue("nonsense"));
      SendJsMessage(message_name,
                    JsArgList(&args), reply_handler.AsWeakHandle());
    }

    {
      ListValue args;
      ListValue* ids = new ListValue();
      args.Append(ids);
      ids->Append(Value::CreateStringValue("0"));
      SendJsMessage(message_name,
                    JsArgList(&args), reply_handler.AsWeakHandle());
    }

    {
      ListValue args;
      ListValue* ids = new ListValue();
      args.Append(ids);
      ids->Append(Value::CreateStringValue("9999"));
      SendJsMessage(message_name,
                    JsArgList(&args), reply_handler.AsWeakHandle());
    }
  }

  void RunGetNodesByIdTest(const char* message_name, bool is_detailed) {
    int64 root_id = kInvalidId;
    {
      ReadTransaction trans(FROM_HERE, sync_manager_.GetUserShare());
      ReadNode root_node(&trans);
      root_node.InitByRootLookup();
      root_id = root_node.GetId();
    }

    int64 child_id =
        MakeNode(sync_manager_.GetUserShare(),
                 syncable::BOOKMARKS, "testtag");

    StrictMock<MockJsReplyHandler> reply_handler;

    JsArgList return_args;

    const int64 ids[] = { root_id, child_id };

    EXPECT_CALL(reply_handler,
                HandleJsReply(message_name, _))
        .Times(arraysize(ids)).WillRepeatedly(SaveArg<1>(&return_args));

    for (size_t i = 0; i < arraysize(ids); ++i) {
      ListValue args;
      ListValue* id_values = new ListValue();
      args.Append(id_values);
      id_values->Append(Value::CreateStringValue(base::Int64ToString(ids[i])));
      SendJsMessage(message_name,
                    JsArgList(&args), reply_handler.AsWeakHandle());

      CheckGetNodesByIdReturnArgs(sync_manager_, return_args,
                                  ids[i], is_detailed);
    }
  }

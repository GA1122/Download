  void OnScriptedPrint(const PrintHostMsg_ScriptedPrint_Params& scripted_params,
                       IPC::Message* reply_msg) {
    manager->OnScriptedPrint(scripted_params, reply_msg);
  }

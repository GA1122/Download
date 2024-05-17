  static v8::Handle<v8::Value> GetNextContextMenuId(const v8::Arguments& args) {
    static int next_context_menu_id = 1;
     return v8::Integer::New(next_context_menu_id++);
   }

int js_isundefined(js_State *J, int idx) { return stackidx(J, idx)->type == JS_TUNDEFINED; }
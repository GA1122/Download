bool jsvIsName(const JsVar *v) { return v && (v->flags&JSV_VARTYPEMASK)>=_JSV_NAME_START && (v->flags&JSV_VARTYPEMASK)<=_JSV_NAME_END; }  

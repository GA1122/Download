 static String* GetString(JSObject* holder) {
    DCHECK(holder->IsJSValue());
 JSValue* js_value = JSValue::cast(holder);
    DCHECK(js_value->value()->IsString());
 return String::cast(js_value->value());
 }

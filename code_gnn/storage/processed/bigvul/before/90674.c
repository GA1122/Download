void jsB_initstring(js_State *J)
{
	J->String_prototype->u.s.string = "";
	J->String_prototype->u.s.length = 0;

	js_pushobject(J, J->String_prototype);
	{
		jsB_propf(J, "String.prototype.toString", Sp_toString, 0);
		jsB_propf(J, "String.prototype.valueOf", Sp_valueOf, 0);
		jsB_propf(J, "String.prototype.charAt", Sp_charAt, 1);
		jsB_propf(J, "String.prototype.charCodeAt", Sp_charCodeAt, 1);
		jsB_propf(J, "String.prototype.concat", Sp_concat, 0);  
		jsB_propf(J, "String.prototype.indexOf", Sp_indexOf, 1);
		jsB_propf(J, "String.prototype.lastIndexOf", Sp_lastIndexOf, 1);
		jsB_propf(J, "String.prototype.localeCompare", Sp_localeCompare, 1);
		jsB_propf(J, "String.prototype.match", Sp_match, 1);
		jsB_propf(J, "String.prototype.replace", Sp_replace, 2);
		jsB_propf(J, "String.prototype.search", Sp_search, 1);
		jsB_propf(J, "String.prototype.slice", Sp_slice, 2);
		jsB_propf(J, "String.prototype.split", Sp_split, 2);
		jsB_propf(J, "String.prototype.substring", Sp_substring, 2);
		jsB_propf(J, "String.prototype.toLowerCase", Sp_toLowerCase, 0);
		jsB_propf(J, "String.prototype.toLocaleLowerCase", Sp_toLowerCase, 0);
		jsB_propf(J, "String.prototype.toUpperCase", Sp_toUpperCase, 0);
		jsB_propf(J, "String.prototype.toLocaleUpperCase", Sp_toUpperCase, 0);

		 
		jsB_propf(J, "String.prototype.trim", Sp_trim, 0);
	}
	js_newcconstructor(J, jsB_String, jsB_new_String, "String", 0);  
	{
		jsB_propf(J, "String.fromCharCode", S_fromCharCode, 0);  
	}
	js_defglobal(J, "String", JS_DONTENUM);
}
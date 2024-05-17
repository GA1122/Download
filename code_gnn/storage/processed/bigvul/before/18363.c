js_init() {
     
    if (!uzbl.js.initialized) {
         
        uzbl.js.classdef = kJSClassDefinitionEmpty;
        uzbl.js.classdef.staticFunctions = js_static_functions;

        uzbl.js.classref = JSClassCreate(&uzbl.js.classdef);
    }
}

NO_INLINE JsVar *jspeFunctionCall(JsVar *function, JsVar *functionName, JsVar *thisArg, bool isParsing, int argCount, JsVar **argPtr) {
  if (JSP_SHOULD_EXECUTE && !function) {
    if (functionName)
      jsExceptionHere(JSET_ERROR, "Function %q not found!", functionName);
    else
      jsExceptionHere(JSET_ERROR, "Function not found!", functionName);
    return 0;
  }

  if (JSP_SHOULD_EXECUTE) if (!jspCheckStackPosition()) return 0;  

  if (JSP_SHOULD_EXECUTE && function) {
    JsVar *returnVar = 0;

    if (!jsvIsFunction(function)) {
      jsExceptionHere(JSET_ERROR, "Expecting a function to call, got %t", function);
      return 0;
    }
    JsVar *thisVar = jsvLockAgainSafe(thisArg);
    if (isParsing) JSP_MATCH('(');

     
    if (jsvIsNative(function)) {  

      unsigned int argPtrSize = 0;
      int boundArgs = 0;
      JsvObjectIterator it;
      jsvObjectIteratorNew(&it, function);
      JsVar *param = jsvObjectIteratorGetKey(&it);
      while (jsvIsFunctionParameter(param)) {
        if ((unsigned)argCount>=argPtrSize) {
          unsigned int newArgPtrSize = argPtrSize?argPtrSize*4:16;
          JsVar **newArgPtr = (JsVar**)alloca(sizeof(JsVar*)*newArgPtrSize);
          memcpy(newArgPtr, argPtr, (unsigned)argCount*sizeof(JsVar*));
          argPtr = newArgPtr;
          argPtrSize = newArgPtrSize;
        }
        int i;
        for (i=argCount-1;i>=boundArgs;i--)
          argPtr[i+1] = argPtr[i];
        argPtr[boundArgs] = jsvSkipName(param);
        argCount++;
        boundArgs++;
        jsvUnLock(param);
        jsvObjectIteratorNext(&it);
        param = jsvObjectIteratorGetKey(&it);
      }
      while (param) {
        if (jsvIsStringEqual(param, JSPARSE_FUNCTION_THIS_NAME)) {
          jsvUnLock(thisVar);
          thisVar = jsvSkipName(param);
          break;
        }
        jsvUnLock(param);
        jsvObjectIteratorNext(&it);
        param = jsvObjectIteratorGetKey(&it);
      }
      jsvUnLock(param);
      jsvObjectIteratorFree(&it);

      int allocatedArgCount = boundArgs;
      if (isParsing) {
        while (!JSP_HAS_ERROR && lex->tk!=')' && lex->tk!=LEX_EOF) {
          if ((unsigned)argCount>=argPtrSize) {
            unsigned int newArgPtrSize = argPtrSize?argPtrSize*4:16;
            JsVar **newArgPtr = (JsVar**)alloca(sizeof(JsVar*)*newArgPtrSize);
            memcpy(newArgPtr, argPtr, (unsigned)argCount*sizeof(JsVar*));
            argPtr = newArgPtr;
            argPtrSize = newArgPtrSize;
          }
          argPtr[argCount++] = jsvSkipNameAndUnLock(jspeAssignmentExpression());
          if (lex->tk!=')') JSP_MATCH_WITH_CLEANUP_AND_RETURN(',',jsvUnLockMany((unsigned)argCount, argPtr);jsvUnLock(thisVar);, 0);
        }

        JSP_MATCH(')');
        allocatedArgCount = argCount;
      }

      void *nativePtr = jsvGetNativeFunctionPtr(function);

      JsVar *oldThisVar = execInfo.thisVar;
      if (thisVar)
        execInfo.thisVar = jsvRef(thisVar);
      else {
        if (nativePtr==jswrap_eval) {  
           
          if (execInfo.thisVar) execInfo.thisVar = jsvRef(execInfo.thisVar);
        } else {
          execInfo.thisVar = jsvRef(execInfo.root);  
        }
      }



      if (nativePtr && !JSP_HAS_ERROR) {
        returnVar = jsnCallFunction(nativePtr, function->varData.native.argTypes, thisVar, argPtr, argCount);
      } else {
        returnVar = 0;
      }

      jsvUnLockMany((unsigned)allocatedArgCount, argPtr);

       
      if (execInfo.thisVar) jsvUnRef(execInfo.thisVar);
      execInfo.thisVar = oldThisVar;

    } else {  
      JsVar *functionRoot = jsvNewWithFlags(JSV_FUNCTION);
      if (!functionRoot) {  
        jspSetError(false);
        jsvUnLock(thisVar);
        return 0;
      }

      JsVar *functionScope = 0;
      JsVar *functionCode = 0;
      JsVar *functionInternalName = 0;
      uint16_t functionLineNumber = 0;

       
      JsvObjectIterator it;
      jsvObjectIteratorNew(&it, function);

      JsVar *param = jsvObjectIteratorGetKey(&it);
      JsVar *value = jsvObjectIteratorGetValue(&it);
      while (jsvIsFunctionParameter(param) && value) {
        JsVar *paramName = jsvNewFromStringVar(param,1,JSVAPPENDSTRINGVAR_MAXLENGTH);
        if (paramName) {  
          jsvMakeFunctionParameter(paramName);  
          jsvSetValueOfName(paramName, value);
          jsvAddName(functionRoot, paramName);
          jsvUnLock(paramName);
        } else
          jspSetError(false);
        jsvUnLock2(value, param);
        jsvObjectIteratorNext(&it);
        param = jsvObjectIteratorGetKey(&it);
        value = jsvObjectIteratorGetValue(&it);
      }
      jsvUnLock2(value, param);
      if (isParsing) {
        int hadParams = 0;
        while (!JSP_SHOULDNT_PARSE && lex->tk!=')') {
          JsVar *param = jsvObjectIteratorGetKey(&it);
          bool paramDefined = jsvIsFunctionParameter(param);
          if (lex->tk!=')' || paramDefined) {
            hadParams++;
            JsVar *value = 0;
            if (lex->tk!=')')
              value = jspeAssignmentExpression();
            value = jsvSkipNameAndUnLock(value);
            JsVar *paramName = paramDefined ? jsvNewFromStringVar(param,1,JSVAPPENDSTRINGVAR_MAXLENGTH) : jsvNewFromEmptyString();
            if (paramName) {  
              jsvMakeFunctionParameter(paramName);  
              jsvSetValueOfName(paramName, value);
              jsvAddName(functionRoot, paramName);
              jsvUnLock(paramName);
            } else
              jspSetError(false);
            jsvUnLock(value);
            if (lex->tk!=')') JSP_MATCH(',');
          }
          jsvUnLock(param);
          if (paramDefined) jsvObjectIteratorNext(&it);
        }
        JSP_MATCH(')');
      } else {   
        int args = 0;
        while (args<argCount) {
          JsVar *param = jsvObjectIteratorGetKey(&it);
          bool paramDefined = jsvIsFunctionParameter(param);
          JsVar *paramName = paramDefined ? jsvNewFromStringVar(param,1,JSVAPPENDSTRINGVAR_MAXLENGTH) : jsvNewFromEmptyString();
          if (paramName) {
            jsvMakeFunctionParameter(paramName);  
            jsvSetValueOfName(paramName, argPtr[args]);
            jsvAddName(functionRoot, paramName);
            jsvUnLock(paramName);
          } else
            jspSetError(false);
          args++;
          jsvUnLock(param);
          if (paramDefined) jsvObjectIteratorNext(&it);
        }
      }
      while (jsvObjectIteratorHasValue(&it)) {
        JsVar *param = jsvObjectIteratorGetKey(&it);
        if (jsvIsString(param)) {
          if (jsvIsStringEqual(param, JSPARSE_FUNCTION_SCOPE_NAME)) functionScope = jsvSkipName(param);
          else if (jsvIsStringEqual(param, JSPARSE_FUNCTION_CODE_NAME)) functionCode = jsvSkipName(param);
          else if (jsvIsStringEqual(param, JSPARSE_FUNCTION_NAME_NAME)) functionInternalName = jsvSkipName(param);
          else if (jsvIsStringEqual(param, JSPARSE_FUNCTION_THIS_NAME)) {
            jsvUnLock(thisVar);
            thisVar = jsvSkipName(param);
          } else if (jsvIsStringEqual(param, JSPARSE_FUNCTION_LINENUMBER_NAME)) functionLineNumber = (uint16_t)jsvGetIntegerAndUnLock(jsvSkipName(param));
          else if (jsvIsFunctionParameter(param)) {
            JsVar *paramName = jsvNewFromStringVar(param,1,JSVAPPENDSTRINGVAR_MAXLENGTH);
            if (paramName) { 
              jsvMakeFunctionParameter(paramName);
              JsVar *defaultVal = jsvSkipName(param);
              if (defaultVal) jsvUnLock(jsvSetValueOfName(paramName, defaultVal));
              jsvAddName(functionRoot, paramName);
              jsvUnLock(paramName);
            }
          }
        }
        jsvUnLock(param);
        jsvObjectIteratorNext(&it);
      }
      jsvObjectIteratorFree(&it);

      if (functionInternalName) {
        JsVar *name = jsvMakeIntoVariableName(jsvNewFromStringVar(functionInternalName,0,JSVAPPENDSTRINGVAR_MAXLENGTH), function);
        jsvAddName(functionRoot, name);
        jsvUnLock2(name, functionInternalName);
      }

      if (!JSP_HAS_ERROR) {
        JsVar *oldScopes[JSPARSE_MAX_SCOPES];
        int oldScopeCount;
        int i;
        oldScopeCount = execInfo.scopeCount;
        for (i=0;i<execInfo.scopeCount;i++)
          oldScopes[i] = execInfo.scopes[i];
        if (functionScope) {
          jspeiLoadScopesFromVar(functionScope);
          jsvUnLock(functionScope);
        } else {
          execInfo.scopeCount = 0;
        }
        if (jspeiAddScope(functionRoot)) {
           

          JsVar *oldThisVar = execInfo.thisVar;
          if (thisVar)
            execInfo.thisVar = jsvRef(thisVar);
          else
            execInfo.thisVar = jsvRef(execInfo.root);  


           
          if (functionCode) {
#ifdef USE_DEBUGGER
            bool hadDebuggerNextLineOnly = false;

            if (execInfo.execute&EXEC_DEBUGGER_STEP_INTO) {
	      if (functionName)
		jsiConsolePrintf("Stepping into %v\n", functionName);
	      else
		jsiConsolePrintf("Stepping into function\n", functionName);
            } else {
              hadDebuggerNextLineOnly = execInfo.execute&EXEC_DEBUGGER_NEXT_LINE;
              if (hadDebuggerNextLineOnly)
                execInfo.execute &= (JsExecFlags)~EXEC_DEBUGGER_NEXT_LINE;
            }
#endif


            JsLex newLex;
            JsLex *oldLex = jslSetLex(&newLex);
            jslInit(functionCode);
            newLex.lineNumberOffset = functionLineNumber;
            JSP_SAVE_EXECUTE();
#ifdef USE_DEBUGGER
            execInfo.execute = EXEC_YES | (execInfo.execute&(EXEC_CTRL_C_MASK|EXEC_ERROR_MASK|EXEC_DEBUGGER_NEXT_LINE));
#else
            execInfo.execute = EXEC_YES | (execInfo.execute&(EXEC_CTRL_C_MASK|EXEC_ERROR_MASK));
#endif
            if (jsvIsFunctionReturn(function)) {
              #ifdef USE_DEBUGGER
                if (execInfo.execute&EXEC_DEBUGGER_NEXT_LINE && JSP_SHOULD_EXECUTE) {
                  lex->tokenLastStart = jsvStringIteratorGetIndex(&lex->tokenStart.it)-1;
                  jsiDebuggerLoop();
                }
              #endif
              if (lex->tk != ';' && lex->tk != '}')
                returnVar = jsvSkipNameAndUnLock(jspeExpression());
            } else {
              JsVar *returnVarName = jsvAddNamedChild(functionRoot, 0, JSPARSE_RETURN_VAR);
              jspeBlockNoBrackets();
               
              returnVar = jsvSkipNameAndUnLock(returnVarName);
              if (returnVarName)  
                jsvSetValueOfName(returnVarName, 0);  
            }
            JsExecFlags hasError = execInfo.execute&EXEC_ERROR_MASK;
            JSP_RESTORE_EXECUTE();  

#ifdef USE_DEBUGGER
            bool calledDebugger = false;
            if (execInfo.execute & EXEC_DEBUGGER_MASK) {
              jsiConsolePrint("Value returned is =");
              jsfPrintJSON(returnVar, JSON_LIMIT | JSON_SOME_NEWLINES | JSON_PRETTY | JSON_SHOW_DEVICES);
              jsiConsolePrintChar('\n');
              if (execInfo.execute & EXEC_DEBUGGER_FINISH_FUNCTION) {
                calledDebugger = true;
                jsiDebuggerLoop();
              }
            }
            if (hadDebuggerNextLineOnly && !calledDebugger)
              execInfo.execute |= EXEC_DEBUGGER_NEXT_LINE;
#endif

            jslKill();
            jslSetLex(oldLex);

            if (hasError) {
              execInfo.execute |= hasError;  
              JsVar *stackTrace = jsvObjectGetChild(execInfo.hiddenRoot, JSPARSE_STACKTRACE_VAR, JSV_STRING_0);
              if (stackTrace) {
                jsvAppendPrintf(stackTrace, jsvIsString(functionName)?"in function %q called from ":
                    "in function called from ", functionName);
                if (lex) {
                  jspAppendStackTrace(stackTrace);
                } else
                  jsvAppendPrintf(stackTrace, "system\n");
                jsvUnLock(stackTrace);
              }
            }
          }

           
          if (execInfo.thisVar) jsvUnRef(execInfo.thisVar);
          execInfo.thisVar = oldThisVar;

          jspeiRemoveScope();
        }

        for (i=0;i<execInfo.scopeCount;i++)
          jsvUnLock(execInfo.scopes[i]);
        for (i=0;i<oldScopeCount;i++)
          execInfo.scopes[i] = oldScopes[i];
        execInfo.scopeCount = oldScopeCount;
      }
      jsvUnLock(functionCode);
      jsvUnLock(functionRoot);
    }

    jsvUnLock(thisVar);

    return returnVar;
  } else if (isParsing) {  
    jspeParseFunctionCallBrackets();
     
    return 0;
  } else return 0;

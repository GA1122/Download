nsPluginInstance::processPlayerRequest()
{
#if 0
     gnash::log_debug(__PRETTY_FUNCTION__);

     log_debug("SCRIPT OBJECT %d: %x", __LINE__, this->getScriptObject());
#endif

    if ( _requestbuf.size() < 4 ) {
        gnash::log_error("Invalid player request (too short): %s", _requestbuf);
        return false;
    }


    std::string& packet = _requestbuf;
    do {
        boost::trim_left(packet);
        if (packet.empty()) {
            return false;
        }

        std::string term = "</invoke>";
        std::string::size_type pos = packet.find(term);
        if (pos == std::string::npos) {
            gnash::log_debug("Incomplete Invoke message. Probably a fragment.");
            return false;
        }
         
        std::string msg = packet.substr(0, pos + term.size());
        boost::shared_ptr<plugin::ExternalInterface::invoke_t> invoke =
            plugin::ExternalInterface::parseInvoke(msg);

        packet.erase(0, msg.size());
        
        if (!invoke) {
            log_error("Failed to parse invoke message: %s", msg);
            return false;
        }

        if (!invoke->name.empty()) {
            gnash::log_debug("Requested method is: %s", invoke->name);
        } else {
            gnash::log_error("Invoke request missing a name to invoke.");
            continue;
        }
        
        if (invoke->name == "getURL") {
            
            assert(invoke->args.size() > 1);
            
            std::string url = NPStringToString(NPVARIANT_TO_STRING(
                                                   invoke->args[0].get()));
#if 0
            gnash::log_debug("Got a getURL() request: %s", url);
#endif
            std::string op = NPStringToString(NPVARIANT_TO_STRING(
                                                  invoke->args[1].get()));
            std::string target;
            
            std::string data;
            
            if (invoke->args.size() >= 3) {
                target = NPStringToString(NPVARIANT_TO_STRING(
                                              invoke->args[2].get()));
            }
            
            if ( target.empty() ) target = "_self";
            
            if (invoke->args.size() == 4) {
                data = NPStringToString(NPVARIANT_TO_STRING(
                                            invoke->args[3].get()));
            }
            if (op == "GET") {
                gnash::log_debug("Asked to getURL '%s' in target %s", url,
                                 target);
                NPN_GetURL(_instance, url.c_str(), target.c_str());
            } else if (op == "POST") {                
                gnash::log_debug("Asked to postURL '%s' this data %s", url,
                                 data);
                NPN_PostURL(_instance, url.c_str(), target.c_str(), data.size(),
                            data.c_str(), false);
            } else {
                log_error("Unexpected op in getURL (expected POST or GET).");
            }
            
            continue; 
        } else if (invoke->name == "fsCommand") {
            
            assert(invoke->args.size() > 1);
            std::string command = NPStringToString(NPVARIANT_TO_STRING(
                                                       invoke->args[0].get()));
            std::string arg = NPStringToString(NPVARIANT_TO_STRING(
                                                   invoke->args[1].get()));            
            std::string name = _name; 
            std::stringstream jsurl;
            jsurl << "javascript:" << name << "_DoFSCommand('" << command
                  << "','" << arg <<"')";
            
            static const char* tgt = "_self";
            
            gnash::log_debug("Calling NPN_GetURL(%s, %s)",
                             jsurl.str(), tgt);
            
            NPN_GetURL(_instance, jsurl.str().c_str(), tgt);
            continue;
        } else if (invoke->name == "addMethod") {
            
            assert(!invoke->args.empty());

            if (!HasScripting()) {
               LOG_ONCE(log_debug("Ignoring addMethod, no scripting."));
               continue;
            }
            std::string method = NPStringToString(NPVARIANT_TO_STRING(
                                                      invoke->args[0].get()));
            NPIdentifier id = NPN_GetStringIdentifier(method.c_str());
            this->getScriptObject()->AddMethod(id, remoteCallback);
            continue;
        }

        if (!HasScripting()) {
           LOG_ONCE(log_debug("Ignoring invoke, no scripting."));
           continue;
        }
        
        NPVariant result;
        VOID_TO_NPVARIANT(result);
        bool invokeResult = false;
        
        if (!invoke->name.empty() && !invoke->args.empty()) {
            const size_t count = invoke->args.size() - 1;
            boost::scoped_array<NPVariant> args(new NPVariant[count]);
            for (size_t i = 0; i < count; ++i) {
                invoke->args[i+1].copy(args[i]);
            }
            
            NPIdentifier id = NPN_GetStringIdentifier(invoke->name.c_str());
            gnash::log_debug("Invoking JavaScript method %s", invoke->name);
            NPObject* windowObject;
            NPN_GetValue(_instance, NPNVWindowNPObject, &windowObject);
            invokeResult=NPN_Invoke(_instance, windowObject, id, args.get(),
                                    count, &result);
            NPN_ReleaseObject(windowObject);
        }
        std::stringstream ss;
        if (invokeResult) {
            ss << plugin::ExternalInterface::convertNPVariant(&result);
            NPN_ReleaseVariantValue(&result);
        } else {
            ss << plugin::ExternalInterface::makeString("Error");
        }
        size_t ret = _scriptObject->writePlayer(ss.str());
        if (ret != ss.str().size()) {
            log_error("Couldn't write the response to Gnash, network problems.");
            return false;
        }
    } while (!packet.empty());
    
    return true;
}

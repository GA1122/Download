 int rpc_type_of_NPPVariable(int variable)
 {
   int type;
   switch (variable) {
    case NPPVpluginNameString:
    case NPPVpluginDescriptionString:
    case NPPVformValue:  
//   case NPPVpluginNativeAccessibleAtkPlugId:
  	type = RPC_TYPE_STRING;
  	break;
    case NPPVpluginWindowSize:
   case NPPVpluginTimerInterval:
 	type = RPC_TYPE_INT32;
 	break;
   case NPPVpluginNeedsXEmbed:
   case NPPVpluginWindowBool:
    case NPPVpluginTransparentBool:
    case NPPVjavascriptPushCallerBool:
    case NPPVpluginKeepLibraryInMemory:
//   case NPPVpluginUrlRequestsDisplayedBool:
//   case NPPVpluginWantsAllNetworkStreams:
//   case NPPVpluginCancelSrcStream:
//   case NPPVSupportsAdvancedKeyHandling:
  	type = RPC_TYPE_BOOLEAN;
  	break;
    case NPPVpluginScriptableNPObject:
 	type = RPC_TYPE_NP_OBJECT;
 	break;
   default:
 	type = RPC_ERROR_GENERIC;
 	break;
   }
   return type;
 }
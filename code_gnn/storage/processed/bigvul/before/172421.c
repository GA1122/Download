inline const char* hiermode_string(int val)
{
 switch(val)
 {
 case HIER_NONE:
 return "No Hier";
 case HIER_P:
 return "Hier-P";
 case HIER_B:
 return "Hier-B";
 case HIER_P_HYBRID:
 return "Hybrid Hier-P";
 default:
 return "No hier";
 }
}

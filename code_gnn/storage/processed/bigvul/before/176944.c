const char* InputDispatcher::Connection::getStatusLabel() const {
 switch (status) {
 case STATUS_NORMAL:
 return "NORMAL";

 case STATUS_BROKEN:
 return "BROKEN";

 case STATUS_ZOMBIE:
 return "ZOMBIE";

 default:
 return "UNKNOWN";
 }
}

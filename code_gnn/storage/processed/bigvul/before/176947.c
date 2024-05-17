const char* InputDispatcher::Connection::getWindowName() const {
 if (inputWindowHandle != NULL) {
 return inputWindowHandle->getName().string();
 }
 if (monitor) {
 return "monitor";
 }
 return "?";
}

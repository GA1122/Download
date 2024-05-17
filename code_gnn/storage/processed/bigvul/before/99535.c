 NPObject* PluginTest::NPN_CreateObject(NPClass* npClass)
 {
     return browser->createobject(m_npp, npClass);
 }                                 

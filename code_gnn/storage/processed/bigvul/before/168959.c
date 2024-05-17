ServiceWorkerDevToolsAgentHost::~ServiceWorkerDevToolsAgentHost() {
   ServiceWorkerDevToolsManager::GetInstance()->AgentHostDestroyed(this);
 }

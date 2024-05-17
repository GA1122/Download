CameraService::Client* CameraService::Client::getClientFromCookie(void* user) {
 BasicClient *basicClient = gCameraService->getClientByIdUnsafe((int) user);
 Client* client = static_cast<Client*>(basicClient);

 if (client == NULL) return NULL;

 if (client->mDestructionStarted) return NULL;

 return client;
}

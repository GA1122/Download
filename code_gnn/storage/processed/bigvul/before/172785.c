static sp<MediaMetadataRetrieverBase> createRetriever(player_type playerType)
{
    sp<MediaMetadataRetrieverBase> p;
 switch (playerType) {
 case STAGEFRIGHT_PLAYER:
 case NU_PLAYER:
 {
            p = new StagefrightMetadataRetriever;
 break;
 }
 default:
            ALOGE("player type %d is not supported",  playerType);
 break;
 }
 if (p == NULL) {
        ALOGE("failed to create a retriever object");
 }
 return p;
}

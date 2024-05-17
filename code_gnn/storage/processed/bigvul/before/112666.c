static void iconLoadDecisionCallback(IconLoadDecision decision, void* context)
{
    static_cast<DocumentLoader*>(context)->continueIconLoadWithDecision(decision);
}

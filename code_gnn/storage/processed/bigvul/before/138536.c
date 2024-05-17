static void callOnMainThreadFunction(WTF::MainThreadFunction function, void* context)
{
    Platform::current()->mainThread()->postTask(FROM_HERE, new MainThreadTaskRunner(function, context));
}

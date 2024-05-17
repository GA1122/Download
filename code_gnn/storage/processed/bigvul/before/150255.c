  mojo::PendingRemote<mojom::InputChannel> CreatePendingRemote() {
    return receiver_.BindNewPipeAndPassRemote();
   }

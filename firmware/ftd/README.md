# OpenThread ThunderBoard Application

This example application exposes OpenThread configuration and management APIs via a simple command-line interface.

## 1. Start the application

If everything goes well, you should see the following on the TBS2 console:

```
> Program started.
[N] Mle-----------: Role disabled -> detached
[N] Mle-----------: Attach attempt 1, AnyPartition reattaching with Active Dataset
[N] Mle-----------: RLOC16 fffe -> dc0c
[N] Mle-----------: Role detached -> child
```

Press the buttons to send UDP messages to the Host Device.
**Important**: In the current state of the program, the host device's IP is hardcoded in the source code.


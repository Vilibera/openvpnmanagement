# openvpnmanagement

An OpenWrt package written in C that connects to the **OpenVPN management interface** over a TCP socket and makes the VPN server status (connected clients, bytes sent/received, connected-since) available on **ubus**. Other router services and the LuCI web UI can then read it.

## Features
- Talks to the OpenVPN management socket (`--management <ip> <port>`)
- Registers a ubus object with methods for querying server and client status
- Reads its settings from UCI
- Logs to syslog and shuts down cleanly on SIGINT/SIGTERM

## Build
Copy `openvpnmanagement/` into the `package/` directory of an OpenWrt SDK and run:

```sh
make menuconfig          # Base system → openvpnmanagement
make package/openvpnmanagement/compile V=s
```

Dependencies: `libubus`, `libubox`, `libuci`, `libblobmsg-json`.

## Usage
```sh
openvpnmanagement -h 127.0.0.1 -p 7505
ubus list            # look for the openvpn object
```

## Tech
C · OpenWrt · ubus · UCI · sockets · argp

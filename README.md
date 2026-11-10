*This project has been created as part of the 42 curriculum by jmeruma, livliege.*

# ft_irc

An IRC server written in C++23, that real IRC clients can connect to and chat on.

## Description

**IRC** (Internet Relay Chat) is a text-based chat protocol from 1988. Clients connect to a server, pick a nickname, join **channels** (group chats whose names start with `#`) and send messages to a channel or to one person.

`ircserv` is our own IRC server. It follows the classic protocol (RFC 1459 / RFC 2812), so standard IRC clients work with it. Our reference client is **[Halloy](https://halloy.chat)**, and plain `nc` works too.

### Features

- **Many clients at once** with a single `epoll` loop. Every socket is non-blocking, and the server never waits on one client.
- **Registration**: a client must send `PASS`, `NICK` and `USER` before it can do anything else.
- **Channels**: join, leave, talk, topics, and channel operators (the first person in a channel becomes its operator).
- **Private messages** to a nickname, and **channel messages** that reach every member.
- **Operator commands**: `KICK`, `INVITE`, `TOPIC` and `MODE` with the modes `i`, `t`, `k`, `o` and `l`.
- **Robust input handling**: messages can arrive in pieces (or several in one read), lines are limited to 512 bytes, and a client that is paused or slow doesn't block anyone else. Outgoing data waits in a per-client send buffer until `epoll` says the socket is writable.

### Supported commands

| Command | What it does |
|---|---|
| `PASS <password>` | Give the server password (required before registration) |
| `NICK <nickname>` | Set or change your nickname (letters, digits, `[]\`_^{\|}-`, max 9 characters, case-insensitive) |
| `USER <username> 0 * :<realname>` | Set your username and real name |
| `JOIN <#chan>[,<#chan>] [<key>[,<key>]]` | Join one or more channels (creates them if they don't exist) |
| `PART <#chan>[,<#chan>] [:<reason>]` | Leave one or more channels |
| `PRIVMSG <target>[,<target>] :<text>` | Send a message to a channel or a nickname |
| `TOPIC <#chan> [:<topic>]` | Show or change the topic of a channel |
| `INVITE <nick> <#chan>` | Invite someone to a channel |
| `KICK <#chan> <nick> [:<reason>]` | Remove someone from a channel (operators only) |
| `MODE <#chan> [+/-modes] [params]` | Show or change the channel modes (operators only) |
| `LIST [<#chan>[,<#chan>]]` | List the channels, with their member count and topic |
| `QUIT [:<reason>]` | Leave the server |
| `PING`, `CAP` | Used automatically by real clients (keep-alive and capability negotiation) |

**Channel modes:**

| Mode | Meaning |
|---|---|
| `+i` | Invite-only: only invited clients can join |
| `+t` | Only operators can change the topic |
| `+k <key>` | Joining needs the key: `JOIN #chan <key>` |
| `+o <nick>` | Give (or with `-o` take away) operator rights |
| `+l <number>` | Maximum number of members |

### Technical choices

- **C++23**: the subject text says C++98, but Codam doesn't enforce it. The [Codam 42 Curriculum page](https://codam.notion.site/42-Curriculum-24e16ae2cc1a8103a2edc3ef8f03d241#2dc16ae2cc1a80649688fc36664c8b80) says under *"Old Core specifics"*:
  > C++ projects: no strict enforcement of C++98 standard, but subject-specific function restrictions apply

  So we use modern C++ (for example structured bindings, `std::optional` and `inline constexpr`), but only the system functions the subject allows.
- **epoll** (so Linux only), level-triggered, in **one** loop for all reading, writing and accepting. Nothing is read or sent unless `epoll` reported that socket as ready.
- **RAII ownership** of file descriptors: an `Fd` class owns exactly one fd and closes it in its destructor. It can be moved but not copied, so no fd is ever leaked or closed twice, even when an exception is thrown.

| Class | Job |
|---|---|
| `Fd` | Owns one file descriptor and closes it exactly once |
| `ListeningSocket` | The server socket: `socket`, `bind`, `listen`, `accept` |
| `ConnectedSocket` | One client connection: receive buffer (splits the data into lines) and send buffer |
| `Epoll` | A small wrapper around `epoll_create1` / `epoll_ctl` / `epoll_wait` |
| `Client` | A connected user: socket, nickname, username, registration state |
| `Channel` | Members (with operator status), invite list, topic, modes, key and limit |
| `Commands` | Maps each command name to the function that handles it |
| `CommandStream` | Builds replies in the IRC format (`:server 001 nick :text`) and sends them to a client or a channel |
| `Server` | Owns everything above and runs the `epoll` loop |

## Instructions

### Requirements

- Linux (the server uses `epoll`)
- A C++ compiler with C++23 support (we use GCC 11.4)
- `make`

### Build

```bash
make          # builds ./ircserv
make clean    # removes the object files
make fclean   # also removes the program
make re       # rebuilds everything
make test     # builds and runs the unit tests
```

### Run

```bash
./ircserv <port> <password>
./ircserv 6667 secret
```

- `port`: the port the server listens on (1–65535)
- `password`: the password every client must give with `PASS`

`make run` starts it with port 6667 and password `secret`. `make vrun` does the same under `valgrind`.

Stop the server with `Ctrl+C`: it closes every connection cleanly.

### Connect with nc

```bash
nc -C localhost 6667
```
`-C` makes `nc` end every line with `\r\n`, as IRC expects. Then register:
```
PASS secret
NICK [nickname]
USER [username] 0 * :[real name]
JOIN #general
PRIVMSG #general :hello everyone!
```

To quickly connect and register with nc
```
irc() { (printf 'PASS secret\nNICK %s\nUSER %s 0 * :%s\n' "$1" "$1" "$1"; cat) | nc -C localhost 6667; } ; irc [name]

// [name] will be set as nickname, username and real name
```

### Connect with Halloy

Add the server to Halloy's `config.toml`:
```toml
[servers.ircserv]
nickname = "halloy"
server = "127.0.0.1"
port = 6667
password = "secret"
use_tls = false
```
Then use `/join #general`, `/msg jisse hi`, `/kick`, `/mode`, `/topic` and so on.

## Resources

### IRC protocol
- [RFC 1459: Internet Relay Chat Protocol](https://datatracker.ietf.org/doc/html/rfc1459)
- [RFC 2812: Internet Relay Chat: Client Protocol](https://datatracker.ietf.org/doc/html/rfc2812)
- [Modern IRC Client Protocol](https://modern.ircdocs.horse/): a clear, up-to-date description of the protocol and the numeric replies
- [Halloy documentation](https://halloy.chat): our reference client

### Network programming
- [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/)
- `man 7 epoll`, `man 2 epoll_wait`, `man 2 socket`, `man 2 accept`, `man 2 recv`, `man 2 send`
- [cppreference.com](https://en.cppreference.com): the C++ standard library

### How AI was used

We used **Claude** (by Anthropic) as a teacher and helper:

- **Learning:** explanations of sockets, `epoll`, non-blocking I/O, RAII and move semantics, the IRC message format and numeric replies, and modern C++ features. It also made study notes and diagrams for reviewing the network part.
- **Planning:** splitting the project into small steps, with hints and questions so we could work out the logic ourselves.
- **Code review and debugging:** reading our code and pointing out bugs (for example use-after-free after a disconnect, dangling `Client*` pointers in channels, and a wrong `MODE` argument queue), explaining compiler errors and `valgrind` output, and checking the code against the subject's rules.
- **Debug prints:** It converted the old debug prints to the new `debug()` function.
- **Git and documentation:** help with branches, merges and conflicts, pull request messages, and a first draft of this README.

We wrote, tested and reviewed the code ourselves, and we can both explain every part of it.

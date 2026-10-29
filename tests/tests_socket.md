# Tests for the Socket Part (Task 1b)

The socket code lives in the **`ListeningSocket` constructor** (`src/ListeningSocket.cpp`).
`Server` has a `ListeningSocket` member, and the fd is owned by an **`Fd`** inside it.

Tick each box when it works. Use two terminals when a test says "terminal 2".

---

## ⚙️ Before you start: keep the server running

Right now the server stops right away (there is no loop yet). Many tests need it to **keep running**.
For those tests, add this **temporary** line in `main`, right after `Server server(argv[1], argv[2]);`:

```cpp
sleep(30); // TEMPORARY: keep the server alive for testing
```

⚠️ Remove it after testing, and **don't commit it**! (Once the epoll loop exists, you won't need it anymore.)

Tests that need it are marked with ⏱️.

Handy: `make run` = `./ircserv 6667 secret`, `make vrun` = the same with `valgrind --track-fds=yes`.

---

## 1. `socket()`

- [ ] **Normal start:** `make run`
  - Expected: `[ListeningSocket] constructor called: listening on 0.0.0.0:6667 with fd 3`
  - Why 3? (0, 1, 2 = stdin, stdout, stderr)
- [ ] **Fail on purpose:** in `ListeningSocket.cpp`, change `AF_INET` in the `socket(...)` call (initializer list) to `12345` for one test.
  - Expected: `An error occurred: Failed to create socket: Address family not supported by protocol`
  - ⚠️ Put `AF_INET` back after the test, and don't commit the test version!

## 2. `setsockopt(SO_REUSEADDR)`

- [ ] **Fast restart:** ⏱️
  1. Terminal 1: `make run`
  2. Terminal 2: `nc -C 127.0.0.1 6667` (connect a client)
  3. Terminal 1: stop the server with `Ctrl+C`
  4. Terminal 1: `make run` again **right away**
  - Expected: it starts with no error.
- [ ] **Bonus:** comment out the `setsockopt` lines and do it again. You should see `Failed to bind socket on port 6667: Address already in use` (for about 1 minute, `TIME_WAIT`).
  - ⚠️ This may only show up **after Task 1c** (`accept()`). A client that was never accepted is cut off with a "reset" when the server stops, and then there is no `TIME_WAIT`. If you see no error now, try again later.

## 3. `fcntl(fd, F_SETFL, O_NONBLOCK)`

- [ ] **Check the flag:** ⏱️ while the server runs, in terminal 2:
  ```sh
  cat /proc/$(pgrep ircserv)/fdinfo/3
  ```
  Look at the `flags:` line. It's an **octal** number. `O_NONBLOCK` is `04000`, so you should see a `4` in that place (for example `04002`).
  - Tip: with `make vrun` the process is called `valgrind`, not `ircserv`, so use `make run` (or `./ircserv 6667 secret`) for this test.
- [ ] Remember the subject rule: `fcntl()` may **only** be called as `fcntl(fd, F_SETFL, O_NONBLOCK)`.
  - ❓ Open question: is `fcntl` allowed on Linux at all? (See Open Questions in `progress.md`.)

## 4. `bind()`

- [x] **Port already in use:** ⏱️ terminal 1: `make run`, terminal 2: `make vrun` (same port).
  - Expected in terminal 2: `An error occurred: Failed to bind socket on port 6667: Address already in use`
  - ✅ Passed on 2026-10-22.
- [ ] **Port below 1024** (not as root): `./ircserv 80 secret`
  - Expected: `An error occurred: Failed to bind socket on port 80: Permission denied`
- [ ] **`INADDR_ANY`:** ⏱️ while the server runs:
  ```sh
  ss -tln | grep 6667
  ```
  Expected: `0.0.0.0:6667` (all interfaces), **not** `127.0.0.1:6667`. The eval checks this!

## 5. `listen()`

- [ ] **The socket is listening:** ⏱️ `ss -tln | grep 6667` shows `LISTEN`.
- [ ] **A client can connect:** ⏱️ `nc -C 127.0.0.1 6667` does **not** say `Connection refused`.
  (Nothing happens yet after connecting. That comes with the epoll loop + `accept()`.)

## 6. Error messages

- [ ] Each error says **which step** failed, for example `Failed to bind socket on port 6667: Address already in use`, not only `Address already in use`.
- [ ] The program never crashes and never calls `exit()` from inside a class. Errors are thrown and caught in `main`.
- [ ] Wrong arguments still give a clear error: `./ircserv abc secret`, `./ircserv 70000 secret`, `./ircserv 6667 ""`, `./ircserv` (no arguments).

## 7. No fd leaks (RAII)

- [ ] **Normal stop:** `make vrun` (without the `sleep`)
  - Expected at the end: `FILE DESCRIPTORS: 3 open (3 std) at exit.`
  - Expected debug lines: `[ListeningSocket] destructor called` and then `[Fd] destructor called: fd 3 is closed` (outside → in).
- [x] **Stop after an error:** the "port already in use" test (section 4) with `make vrun` as the **2nd** server.
  - Expected: `[Fd] destructor called: fd 3 is closed` **before** the error, and `3 open (3 std) at exit`.
  - There is **no** `[ListeningSocket] destructor called` line: its constructor threw, so its destructor doesn't run. But the `Fd` member was already finished, so **its** destructor runs and closes fd 3.
  - ✅ Passed on 2026-10-22.
- [ ] ⚠️ Note: with `Ctrl+C` the program is killed by a signal, so no destructor runs (valgrind will show fd 3 open). That's something for later (signal handling).

## 8. Before handing in

- [ ] `-Werror` is turned back on in the Makefile, and `make re` still compiles.
- [ ] No temporary `sleep(...)` or test code left in `main`.

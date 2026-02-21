# SQRL

SQRL is a local-first desktop credential vault built with C++/wxWidgets. It runs entirely on your machine, uses a post-quantum hybrid local handshake, stores encrypted data at rest, and supports both password and iris-assisted login flows.

## What SQRL does

- Local account creation with `User ID + Password`.
- Live iris enrollment at signup.
- Login with password (`Submit`) or iris (`Login with Iris`).
- Encrypted local record storage using OpenSSL AES-256-GCM.
- Local post-quantum hybrid handshake (`ML-KEM-768 + X25519`, signed by `ML-DSA-65`).

## Stack

- `C++17`
- `wxWidgets` (desktop UI + socket events)
- `OpenSSL 3` (PQ algorithms, PBKDF2, AES-GCM, HMAC, signatures)
- `Crypto++` (existing project utilities)
- `Python 3 + OpenCV` (`iris_capture.py` live capture helper)

## Repository map

- `App.cpp` / `App.h`: app bootstrap and local server startup.
- `Server.cpp` / `Server.h`: local socket server and PQ hybrid handshake.
- `LoginFrame.cpp` / `LoginFrame.h`: signup/login UI and auth flow.
- `MainFrame.cpp` / `MainFrame.h`: encrypted data UI (add/read/clear/signout).
- `iris_capture.py`: live camera capture and iris template output.
- `COMPILE_CMD.txt`: canonical local build command.
- `MIGRATION.md`: storage format history and migration/reset notes.

## Prerequisites

- `wxWidgets`
- `Crypto++`
- `OpenSSL@3`
- `Python 3`

macOS example:

```bash
brew install wxwidgets cryptopp openssl@3
```

## Python setup (for iris capture)

Create a project-local venv and install OpenCV:

```bash
cd /Users/akbarshamji/Development/SQRL
python3 -m venv .venv
.venv/bin/python -m pip install opencv-python
```

SQRL automatically prefers `.venv/bin/python3` when launching iris capture.

## Build

Use the included build command file:

```bash
cd /Users/akbarshamji/Development/SQRL
sh COMPILE_CMD.txt
```

## Run

```bash
cd /Users/akbarshamji/Development/SQRL
./sqrl
```

SQRL binds locally on `localhost`, starting at port `3000` with automatic fallback.

## Using SQRL

### Signup flow

1. Start SQRL.
2. Keep `New user (Signup)` selected.
3. Enter `User ID` and `Password`.
4. Click `Capture Iris (Live)` and complete capture in Terminal/camera window.
5. Click `Submit`.

### Login with password

1. Select `Existing user (Login)`.
2. Enter `User ID` and `Password`.
3. Click `Submit`.

### Login with iris

1. Select `Existing user (Login)`.
2. Enter `User ID`.
3. Click `Login with Iris`.
4. Complete live capture in Terminal/camera window.

### Sign out

From the main frame, click `Sign out` to return to login UI.

## Local data files

### Account/auth file

`login_details.txt` stores one line per user:

```text
<user_id>|<auth_salt_hex>|<auth_hash_hex>|<encryption_salt_hex>|<signup_iris_hash>|<key_mode>
```

### Encrypted data file

Per-user data is stored in:

```text
<SHA256(user_id)>.txt
```

Each line:

```text
<plain_id>|<nonce_b64>|<tag_b64>|<ciphertext_b64>
```

## Verification commands

Confirm PQ algorithms are visible to OpenSSL:

```bash
openssl list -public-key-algorithms | rg "ML-KEM|ML-DSA|X25519"
```

Confirm local app listener:

```bash
lsof -iTCP -sTCP:LISTEN -nP | rg "sqrl|3000|3001"
```

## Troubleshooting

- Rebuild after code updates:

```bash
sh COMPILE_CMD.txt
```

- Restart app cleanly:

```bash
pkill -f '/Users/akbarshamji/Development/SQRL/sqrl' || true
./sqrl
```

- If migrating older local data, follow `MIGRATION.md`.

## Contributing

1. Fork and clone.
2. Build locally with `sh COMPILE_CMD.txt`.
3. Test signup/login flows.
4. Open a PR with a concise change summary.

## Migration

For storage format updates and reset guidance:
- `MIGRATION.md`

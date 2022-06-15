# XRPL Hooks Debug

## XRPL Hooks Testnet

Docker Compose the xrpl testnet (Can also just change the code to connect to the public RPC)

Repo: https://github.com/XRPL-Labs/xrpld-hooks
Testnet: https://hooks-testnet.xrpl-labs.com
Documentation: https://hooks-testnet.xrpl-labs.com

`docker-compose up -d --build`

## Install WASMCC

# Install

`curl https://raw.githubusercontent.com/wasienv/wasienv/master/install.sh | sh`

# Manually Add Export (If Error)

`sudo nano ~/.zshrc`

```
export WASIENV_DIR="/Users/[username]/.wasienv"
[ -s "$WASIENV_DIR/wasienv.sh" ] && source "$WASIENV_DIR/wasienv.sh"
```

`source ~/.zshrc`

## Install venv

We use virtualenv. Install with [sudo] pip install virtualenv, initialize with virtualenv venv, and activate with source venv/bin/activate.

Install the development requirements with `pip install -r requirements.txt`

# Workflow (Single Hook)

This repo is set up for a single hook use case, if you have a multi hook use case then there are a few adjustments you will need to make. See `#Workflow (Multi Hook)`

## Step 0

Make changes in `src/lib.c`

## Step 1

Run `python3 test.py tests/`

The script will run the following commands.

1. Init Treasury Accounts (Default 1)
2. Init Accounts (Default 3)
3. Fund Accounts (Default 1000 XRP)
4. Build Hooks (Default `hook_debug`)
5. Set Hook (Default `hook_debug`)
6. Test Hook (Default `hook_debug`)


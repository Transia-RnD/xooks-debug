
# parent_path=$( cd "$(dirname "${BASH_SOURCE[0]}")" ; pwd -P )
# cd "$parent_path"
#
# src_release=target/wasm32-unknown-unknown/release/
# src_debug=target/wasm32-unknown-unknown/debug/
# dst=.

# cargo build --lib && \
# cargo build --lib --release && \
# cp -v "$src_debug"/*.wasm "$dst"/hook_debug.wasm && \
# cp -v "$src_release"/*.wasm "$dst"/hook.wasm

all:
	wasmcc ./src/lib.c -o ./build/hook_debug.wasm -O0 -Wl,--allow-undefined -I../
	# wasm-opt -O2 hook_debug.wasm -o hook_debug.wasm
	# hook-cleaner hook_debug.wasm
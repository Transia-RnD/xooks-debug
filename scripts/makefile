
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
	wasmcc ../src/contracts/tax-cur-in.c -o ../dist/taxcurin.wasm -O0 -Wl,--allow-undefined -I../
	wasmcc ../src/contracts/tax-cur-out.c -o ../dist/taxcurout.wasm -O0 -Wl,--allow-undefined -I../
	wasmcc ../src/contracts/tax-xrp-in.c -o ../dist/taxxrpin.wasm -O0 -Wl,--allow-undefined -I../
	wasmcc ../src/contracts/tax-xrp-out.c -o ../dist/taxxrpout.wasm -O0 -Wl,--allow-undefined -I../

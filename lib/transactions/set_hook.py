#!/usr/bin/env python
# coding: utf-8


import os
import binascii
from sys import flags
import time
from typing import List, Optional

from xrpl.clients import WebsocketClient
from xrpl.wallet import Wallet
from xrpl.models.transactions import SetHook, Hook, HookFlag
from xrpl.transaction import (
    send_reliable_submission,
    safe_sign_and_autofill_transaction,
    get_transaction_from_hash,
)
from xrpl.core.binarycodec import decode
# from xrpl.ledger import get_latest_validated_ledger_sequence
# from xrpl.account import get_next_valid_seq_number

BASE_DIR = os.path.abspath(os.path.join(os.path.dirname(__file__)))

def set_hook(
    path: str,
    w3: WebsocketClient,
    wallet: Wallet,
):
    hook_account = wallet.classic_address
    print('CONNECTED')

    with open(path, 'rb') as f:
        content = f.read()
    
    binary = binascii.hexlify(content).decode('utf-8').upper()

    print('BUILDING...')
    built_transaction = SetHook(
        account=hook_account,
        hooks=[
            Hook(
                create_code=binary,
                hook_on='0000000000000000',
                hook_namespace='E3B0C44298FC1C149AFBF4C8996FB92427AE41E4649B934CA495991B7852B855',
                flags=HookFlag.TF_OVERRIDE,
                hook_api_version=0
            ),
        ],
    )
    # print(built_transaction.to_dict())
    # print(built_transaction.to_xrpl())

    print('GETTING FEE...')
    
    print('SIGNING...')
    signed_tx = safe_sign_and_autofill_transaction(
        transaction=built_transaction,
        wallet=wallet,
        client=w3,
    )
    print(signed_tx.to_xrpl())
    print(decode('1D0A0BA86EDBC68AC850BE176ED79B8C88D6A9DC2A6C9BEEE2AFA0D317FD0F70743D2BB43BCB680F5915F064C31704A21C3C9144DBEB2EA0796D2E279E47A50D'))
    # response = send_reliable_submission(signed_tx, w3)
    # tx_result = response.result['meta']['TransactionResult']
    # print('TX HASH: {}'.format(response.result['hash']))
    # print(
    #     '{} The hook was set. Only final in a validated ledger.'.format(tx_result)  # noqa
    # )
    # for i in range(1):
    #     print('{} ...'.format(i))
    #     time.sleep(1)
    # hook_tx = get_transaction_from_hash(response.result["hash"], w3)
    # print(hook_tx)
    # print('CLOSING...')
    print(e)
    w3.close()

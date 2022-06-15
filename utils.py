"""utils.py."""

import json
from xrpl.core import keypairs
from xrpl.wallet import Wallet
from xrpl.models.requests.account_lines import AccountLines
from xrpl.utils import xrp_to_drops
from xrpl.models.transactions import Payment
from xrpl.transaction import (
    send_reliable_submission,
    safe_sign_and_autofill_transaction
)


def read_json(path):
    """read_json."""
    with open(path) as json_file:
        return json.load(json_file)


def write_json(data, path):
    """write_json."""
    with open(path, 'w') as json_file:
        json.dump(data, json_file)


def get_currency_line(w3, currency, issuer, address):
    """get_currency_line."""
    nline = {}
    acct_lines = AccountLines(
        account=address,
        ledger_index="current",
        limit=1000,
    )
    response = w3.request(acct_lines)
    for line in response.result['lines']:
        if line['currency'] != currency:
            # print('SKIPPING WRONG CURRENCY')
            continue
        if line['account'] != issuer:
            # print('SKIPPING WRONG ISSUER')
            continue
        nline['balance'] = int(abs(float(line['balance'])))
        nline['limit'] = int(abs(float(line['limit'])))
    return nline


def pay_xrp(w3, dest, amount, wallet):
    """pay_xrp."""
    drop_value = xrp_to_drops(float(amount))
    built_transaction = Payment(
        account=wallet.classic_address,
        amount=drop_value,
        destination=dest,
    )

    # print(built_transaction)
    signed_tx = safe_sign_and_autofill_transaction(
        transaction=built_transaction,
        wallet=wallet,
        client=w3,
    )
    response = send_reliable_submission(signed_tx, w3)
    tx_result = response.result['meta']['TransactionResult']
    print('TX HASH: {}'.format(response.result['hash']))
    print(
        '{} The transaction was applied. \
        Only final in a validated ledger.'.format(tx_result)
    )
    return response.result['hash']


def gen_new_acct(w3):
    """gen_new_acct."""

    wallet = Wallet('shmJ2RXrBe8PCGTiHuhpyRQMAoVcq', 0)
    # seed = 'shcmfSvm637GXsLC2A6oJ6sGDeg4D'

    seed = keypairs.generate_seed()
    public, private = keypairs.derive_keypair(seed)
    test_account = keypairs.derive_classic_address(public)
    print({'publicKey': test_account, 'seed': seed})
    print('ADDRESS: {}'.format(test_account))
    print('PUBLIC: {}'.format(public))
    print('PRIVATE: {}'.format(private))
    print('SEED: {}'.format(seed))

    pay_xrp(
        w3,
        test_account,
        100,
        wallet,
    )
    return seed

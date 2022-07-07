#!/usr/bin/env python
# coding: utf-8

import logging
import subprocess
import os
from typing import List

from testing_config import BaseTestConfig

from xrpl.clients import WebsocketClient
from xrpl.wallet import Wallet as XrplWallet

from xrpl.models.requests.account_info import AccountInfo
from xrpl.models.transactions.payment import Payment
from xrpl.models.amounts.issued_currency_amount import IssuedCurrencyAmount
from xrpl.transaction import (
    safe_sign_and_autofill_transaction,
    send_reliable_submission
)
from xrpl.utils import xrp_to_drops, drops_to_xrp

from lib import Wallet, read_json, write_json
from lib.config import Config
from lib.transactions import set_hook

XRPL_CHAIN_WSS = Config.XRPL_CHAIN_WSS

logger = logging.getLogger('app')

BASE_DIR = os.path.dirname(__file__)

# import pytest
# @pytest.mark.skip(reason="Using Prod Cert")
class TestHookDebug(BaseTestConfig):
    """TestHookDebug."""

    w3: WebsocketClient

    min_balance: int = 1000

    t_dir: str = os.path.join(BASE_DIR, 'treasury/')
    t_number: int = 1
    t_wallets: List[Wallet] = []

    dir: str = os.path.join(BASE_DIR, 'keystores/')
    number: int = 3
    wallets: List[Wallet] = []

    hooks: List[str ]= ['hook_debug.wasm']
    
    def test_hook_debug(cls):
        client = WebsocketClient(XRPL_CHAIN_WSS)
        with client as w3:
            cls.w3 = w3
            print('1')
            cls.init_treasury()
            print('2')
            cls.init_accounts()
            print('3')
            cls.verify_funded()
            print('4')
            cls.build_hooks()
            print('5')
            cls.set_hooks()
            print('6')
        client.close()

    def build_scripts(cls):
        try:
            dir: str = os.path.abspath(os.path.join(os.path.dirname( __file__ ), '../scripts', 'build'))
            p = subprocess.Popen(["make"], stdout=subprocess.PIPE, cwd=dir)
            p.kill()
        except Exception as e:
            print(e)
            raise e

    def build_hooks(cls):
        try:
            dir: str = os.path.abspath(os.path.join(os.path.dirname( __file__ ), '..', 'build'))
            p = subprocess.Popen(["make"], stdout=subprocess.PIPE, cwd=dir)
            p.kill()
        except Exception as e:
            print(e)
            raise e
    
    def set_hooks(cls):
        dir: str = os.path.abspath(os.path.join(os.path.dirname( __file__ ), '..', 'build'))
        for filename in os.listdir(dir):
            path = os.path.join(dir, filename)
            set_hook(
                path=path,
                w3=cls.w3,
                wallet=cls.wallets[0].wallet,
            )

    def verify_funded(cls):
        try:
            print(f'[HOOK DEBUG] VERIFYING FUNDED ACCOUNTS')
            count: int = 0
            for wallet in cls.wallets:
                account: str = wallet.wallet.classic_address
                acct_info = AccountInfo(
                    account=account,
                    ledger_index="current",
                    queue=True,
                    strict=True,
                )
                response = cls.w3.request(acct_info)
                if 'error' in response.result:
                    if response.result['error'] == 'actNotFound':
                        cls.fund_account(account=account)
                        continue
                
                print(response.result)
                balance: int = drops_to_xrp(response.result['account_data']['Balance'])
                if balance < cls.min_balance:
                    cls.fund_account(account=account)
                    continue
                
                count += 1
                cls.assertTrue(len(cls.wallets), count)
                print(f'[HOOK DEBUG] FUNDED {count}')
            return 200
        except Exception as e:
            print(e)
            raise e

    def fund_account(cls, account: str):
        try:
            print(f'[HOOK DEBUG] FUNDING ACCOUNT | {account}')
            total_xrp = cls.min_balance
            drop_value = xrp_to_drops(float(total_xrp))
            send_token_tx = Payment(
                account=cls.t_wallets[0].wallet.classic_address,
                destination=account,
                amount=drop_value
            )
            pay_prepared = safe_sign_and_autofill_transaction(
                transaction=send_token_tx,
                wallet=cls.t_wallets[0].wallet,
                client=cls.w3,
            )
            print(f"SENDING {total_xrp} TO {account} ...")
            response = send_reliable_submission(pay_prepared, cls.w3)
            # print(response)
            return 200
        except Exception as e:
            print(e)
            raise e

    def init_treasury(cls):
        try:
            print('[HOOK DEBUG]: INITIALIZING TREASURY')
            for filename in os.listdir(cls.t_dir):
                path = os.path.join(cls.t_dir, filename)
                wallet: Wallet = Wallet(dir=cls.t_dir).read(path=path)
                cls.t_wallets.append(wallet)
                
            if len(cls.t_wallets) == 0:
                missing_wallets: int = cls.t_number - len(cls.t_wallets)
                print(f'[HOOK DEBUG]: MISSING TREASURY: {missing_wallets}')
                cls.create_t_accounts(cls.t_dir, cls.t_number)
           
            print(f'[HOOK DEBUG]: TREASURY WALLETS: {len(cls.t_wallets)}')
            cls.assertTrue(len(cls.t_wallets), cls.t_number)
        except Exception as e:
            print(e)
            raise e
        

    def init_accounts(cls):
        try:
            print('[HOOK DEBUG]: INITIALIZING WALLETS')
            for filename in os.listdir(cls.dir):
                path = os.path.join(cls.dir, filename)
                wallet: Wallet = Wallet(dir=cls.dir).read(path=path)
                cls.wallets.append(wallet)
                
            if len(cls.wallets) != cls.number:
                missing_wallets: int = cls.number - len(cls.wallets)
                print(f'[HOOK DEBUG]: MISSING WALLETS: {missing_wallets}')
                cls.create_accounts(cls.dir, cls.number)
           
            print(f'[HOOK DEBUG]: WALLETS: {len(cls.wallets)}')
            cls.assertTrue(len(cls.wallets), cls.number)
        except Exception as e:
            print(e)
            raise e

    def create_accounts(
        cls, 
        dir: str,
        number: int
    ):
        try:
            print(f'[HOOK DEBUG]: CREATING {cls.number} WALLETS')
            for i in range(number):
                print(f'[HOOK DEBUG]: WALLET {i}')
                wallet: Wallet = Wallet(
                    dir=dir
                ).write()
                cls.wallets.append(wallet)
        except Exception as e:
            print(e)
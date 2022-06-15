#!/usr/bin/env python
# coding: utf-8

import logging
import os
import json
from typing import List

from xrpl.core import keypairs
from xrpl.clients import WebsocketClient
from xrpl.wallet import (
    generate_faucet_wallet,
    Wallet as XrplWallet
)

from .config import Config

XRPL_CHAIN_WSS = Config.XRPL_CHAIN_WSS

# Create Logger
logger = logging.getLogger('app')


class Wallet(object):

    client: WebsocketClient
    wallet: XrplWallet
    dir: str

    def __init__(cls, dir: str):
        """
        Initializes the Wallet
        :return: Wallet
        """
        print('[WALLET: __init__')
        cls.client =  WebsocketClient(XRPL_CHAIN_WSS)
        cls.dir = dir

    def write(cls):
        """
        Writes json to file path
        :return: Dict[str, object]
        """
        with cls.client as w3:
            # test_wallet: XrplWallet = generate_faucet_wallet(w3)
            seed = keypairs.generate_seed()
            cls.wallet = XrplWallet(seed, 0)
            address: str = cls.wallet.classic_address
            seed: str = cls.wallet.seed

            json_data = {
                'address': address,
                'version': 'v1',
                'epk': seed,
            }
            with open(os.path.join(cls.dir, f'keystore-{address}.json'), 'w') as outfile:
                json.dump(json_data, outfile)
                
            return cls

    def read(cls, path: str):
        """
        Reads json from file path
        :return: Dict[str, object]
        """
        with open(path) as json_file:
            json_data = json.load(json_file)
            address = json_data['address']
            seed = json_data['epk']
            cls.wallet = XrplWallet(seed, 0)
            return cls

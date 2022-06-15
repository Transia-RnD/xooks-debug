#!/usr/bin/env python
# coding: utf-8

import os
import json

from dotenv import load_dotenv
load_dotenv()

BASE_DIR = os.path.dirname(__file__)

class Config(object):
    """Config."""

    def get_env_var(key, configs=os.environ):
        """get_env_var."""
        try:
            val = configs[key]
            if val == 'True':
                val = True
            elif val == 'False':
                val = False
            return val
        except KeyError:
            error_msg = "ImproperlyConfigured: Set {0} environment variable".format(key)  # noqa
            raise ValueError(error_msg)

    XRPL_CHAIN_WSS = get_env_var('XRPL_CHAIN_WSS')
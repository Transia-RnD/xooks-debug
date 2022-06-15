#!/usr/bin/env python
# coding: utf-8

from unittest import TestCase

# from common.util import read_json


class BaseTestConfig(TestCase):
    """BaseTestConfig."""

    json_fixtures = {}

    @classmethod
    def setUpDelta(cls):
        """setUpDelta."""
        print('setUpDelta')

    @classmethod
    def setUpUser(cls):
        """setUpUser."""
        print('setUpUser')

    @classmethod
    def setUpClass(cls):
        """setUpClass."""
        print('setUpClass')
        cls.setUpUser()

    @classmethod
    def tearDownClass(cls):
        """tearDownClass."""
        print('tearDownClass')

    def setUp(self):
        """setUp."""
        print('setUp')

    def tearDown(self):
        """tearDown."""
        print('tearDown')

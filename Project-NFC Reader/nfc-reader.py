#!/usr/bin/python

import nfc

def on_startup(targets):
    return targets

def on_connect(tag):
    print(tag.identifier.encode('hex_codec'))

rdwr_options = {
    'targets': ['106A'],
    'on-startup': on_startup,
    'on-connect': on_connect,
}
with nfc.ContactlessFrontend('tty:USB0:pn532') as clf:
    tag = clf.connect(rdwr=rdwr_options)
    if tag.ndef:
        print(tag.ndef.message.pretty())
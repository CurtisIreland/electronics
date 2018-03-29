#!/usr/bin/env python
import wx
import wx.lib.newevent
import nfc
import threading
from binascii import hexlify

ShowCardEvent, SHOW_CARD_EVENT = wx.lib.newevent.NewEvent()
GoneCardEvent, GONE_CARD_EVENT = wx.lib.newevent.NewEvent()


class TagReader(threading.Thread):
    def __init__(self, wx_frame):
        super(TagReader, self).__init__(name="TagReader")
        self.terminate = False
        self.wx_frame = wx_frame

    def run(self):
        clf = nfc.ContactlessFrontend('tty:USB0:pn532')
        rdwr_options = {
            'on-connect': self.on_tag_connect,
#            'on-release': self.on_tag_release
        }
        while not self.terminate:
            clf.connect(rdwr=rdwr_options, terminate=lambda: self.terminate)
        clf.close()

    def on_tag_connect(self, tag):
        wx.PostEvent(self.wx_frame, ShowCardEvent(tag=tag))
        return True

    def on_tag_release(self, tag):
        wx.PostEvent(self.wx_frame, GoneCardEvent())


class Frame(wx.Frame):
    def __init__(self, title):
        super(Frame, self).__init__(None, title=title, size=(500, 200))
        self.text = wx.StaticText(wx.Panel(self), pos=(100, 100))
        self.Bind(SHOW_CARD_EVENT, self.show_card_event)
        self.Bind(GONE_CARD_EVENT, self.gone_card_event)
        self.Bind(wx.EVT_CLOSE, self.close_window_event)
        wx.PostEvent(self, GoneCardEvent())
        self.Show()

    def close_window_event(self, event):
        self.Destroy()

    def show_card_event(self, event):
        self.text.SetLabel("Card ID {}".format(hexlify(event.tag.identifier)))

    def gone_card_event(self, event):
        self.text.SetLabel("no card")


app = wx.App()
reader = TagReader(Frame('card reader app'))
reader.start()
app.MainLoop()
reader.terminate = True  # tell the reader to terminate
reader.join()  # and wait for the reader thread to finish

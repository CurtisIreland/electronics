#!/usr/bin/python

import wx
import time
import wx.lib.newevent
import nfc
import threading
from binascii import hexlify

import clock_db

ShowCardEvent, SHOW_CARD_EVENT = wx.lib.newevent.NewEvent()

class TagReader(threading.Thread):
    def __init__(self, wx_frame):
        super(TagReader, self).__init__(name="TagReader")
        self.terminate = False
        self.wx_frame = wx_frame

    def run(self):
        clf = nfc.ContactlessFrontend('tty:USB0:pn532')
        rdwr_options = {
            'on-connect': self.on_tag_connect,
        }
        while not self.terminate:
            clf.connect(rdwr=rdwr_options, terminate=lambda: self.terminate)
        print("Closing Connection")
        clf.close()

    def on_tag_connect(self, tag):
        wx.PostEvent(self.wx_frame, ShowCardEvent(tag=tag))
        time.sleep(1)
        return True


class Punchclock(wx.Frame):
    def __init__(self):
        self.card_name = ''

        wx.Frame.__init__(self, None, title="Punchclock", size=(800, 480))
        self.InitUI()
#        self.Centre()


        self.Show()

        # This makes the application full screen with no title, minimize buttons, etc.
#        self.ShowFullScreen(True)

    def InitUI(self):
        panel = wx.Panel(self)
        panel.SetFont( wx.Font( 16, wx.FONTFAMILY_DEFAULT, wx.FONTSTYLE_NORMAL, wx.FONTWEIGHT_BOLD, False, wx.EmptyString ) )

        self.Bind(SHOW_CARD_EVENT, self.show_card_event)
        
        vbox = wx.BoxSizer( wx.VERTICAL )
        
        hbox1 = wx.BoxSizer( wx.HORIZONTAL )
        
        hbox1.SetMinSize( wx.Size( 0,0 ) ) 
        self.curtime = wx.StaticText( panel, wx.ID_ANY, time.strftime('%A %B %d, %Y\n%-I:%M:%S %p'), wx.DefaultPosition, wx.DefaultSize, wx.ALIGN_CENTRE )
        self.curtime.Wrap( -1 )
        self.curtime.SetFont( wx.Font( 16, wx.FONTFAMILY_DEFAULT, wx.FONTSTYLE_NORMAL, wx.FONTWEIGHT_BOLD, False, wx.EmptyString ) )
        hbox1.Add( self.curtime, 0, wx.ALL, 5 )
        vbox.Add( hbox1, 0, wx.ALIGN_CENTER|wx.TOP, 50 )
        
        vbox.Add( ( 0, 25), 0, 0, 5 )
        
        bp_box = wx.BoxSizer( wx.HORIZONTAL )
        self.button_si = wx.Button( panel, wx.ID_ANY, u"Sign &In", wx.DefaultPosition, wx.DefaultSize, 0 )
        bp_box.Add( self.button_si, 0, wx.ALL, 5 )
        self.Bind(wx.EVT_BUTTON, self.si_message, self.button_si)

        self.button_so = wx.Button( panel, wx.ID_ANY, u"Sign &Out", wx.DefaultPosition, wx.DefaultSize, 0 )
        bp_box.Add( self.button_so, 0, wx.ALL, 5 )
        self.Bind(wx.EVT_BUTTON, self.so_message, self.button_so)

        vbox.Add( bp_box, 0, wx.ALIGN_CENTER, 5 )
        self.ToggleButtons(False)
        
        hbox3 = wx.BoxSizer( wx.HORIZONTAL )
        self.divide_line = wx.StaticLine( panel, wx.ID_ANY, wx.DefaultPosition, wx.Size( 700,1 ), wx.LI_HORIZONTAL )
        hbox3.Add( self.divide_line, 0, wx.ALIGN_BOTTOM|wx.LEFT|wx.RIGHT, 50 )
        vbox.Add( hbox3, 1, wx.ALIGN_BOTTOM|wx.LEFT|wx.RIGHT, 50 )
        
        vbox.Add( ( 0, 10), 0, 0, 5 )
        
        hbox4 = wx.BoxSizer( wx.HORIZONTAL )
        self.status_line = wx.StaticText( panel, wx.ID_ANY, u"Please Tap Your Card", wx.DefaultPosition, wx.DefaultSize, 0 )
        self.status_line.Wrap( -1 )
        self.status_line.SetFont( wx.Font( 16, wx.FONTFAMILY_DEFAULT, wx.FONTSTYLE_NORMAL, wx.FONTWEIGHT_BOLD, False, wx.EmptyString ) )
        hbox4.Add( self.status_line, 0, wx.ALIGN_BOTTOM, 5 )
        vbox.Add( hbox4, 0, wx.ALIGN_BOTTOM|wx.LEFT, 50 )
        
        vbox.Add( ( 0, 25), 0, 0, 5 )
        
        panel.SetSizer( vbox )
        panel.Layout()

        self.OnTimer(None)
        self.timer = wx.Timer(self, -1)
        self.timer.Start(1000)
        self.Bind(wx.EVT_TIMER, self.OnTimer)

        panel.SetSizer(vbox)

    def si_message(self, event):
        check_time = time.strftime('%Y-%m-%d %H:%M:%S')
        self.status_line.SetLabel("Signed in: " + self.card_name + " " + check_time)

        clock_data = clock_db.clock_db()
        self.card_name = clock_data.checkin(self.card_name, "IN", check_time)
        clock_data.close()

        self.card_name = ''
        self.ToggleButtons(False)

    def so_message(self, event):
        check_time = time.strftime('%Y-%m-%d %H:%M:%S')
        self.status_line.SetLabel("Signed out: " + self.card_name + " " + check_time)

        clock_data = clock_db.clock_db()
        self.card_name = clock_data.checkin(self.card_name, "OUT", check_time)
        clock_data.close()

        self.card_name = ''
        self.ToggleButtons(False)

    def ToggleButtons(self, state):
        if (not state):
            print("Turning off buttons...")
            self.button_si.Disable()
            self.button_so.Disable()
        else:
            print("Turning on buttons...")
            self.button_si.Enable()
            self.button_so.Enable()

    def OnTimer(self, event):
        current = time.localtime(time.time())
        ts = time.strftime("%A %B %d, %Y\n%-I:%M:%S %p", current)
        self.curtime.SetLabel(ts)

    def show_card_event(self, event):
        #card_read = tag.identifier.encode('hex_codec')
        card_read = hexlify(event.tag.identifier)
        print("Scanned Card: " + card_read)

        clock_data = clock_db.clock_db()
        self.card_name = clock_data.get_employee(card_read)
        clock_data.close()

        print("Card Owner: " + self.card_name)
        if(self.card_name == ""):
            self.status_line.SetLabel("Unknown Card")
            self.ToggleButtons(False)
        else:
            self.status_line.SetLabel("Scanned: " + self.card_name)
            self.ToggleButtons(True)


if __name__ == "__main__":
    app = wx.App(False)
    reader = TagReader(Punchclock())
    reader.start()
    app.MainLoop()
    reader.terminate = True  # tell the reader to terminate
    reader.join()  # and wait for the reader thread to finish

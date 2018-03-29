import Tkinter as tk
import tkMessageBox
import time
import nfc

import clock_db

time1 = ''
card_read = ''
card_name = ''

def scan_card(tag):
    global card_read, card_name
#    card_read = 'db4e7a3f'
    card_read = tag.identifier.encode('hex_codec')

    clock_data = clock_db.clock_db()
    card_name = clock_data.get_employee(card_read)
    clock_data.close()

    si_status.config(text="Scanned: " + card_name)
    button_frame.place(button_frame.pi)
    button_frame.visible = not button_frame.visible
#    btnToggle.place_forget()

rdwr_options = {
    'targets': ['106A'],
    'on-connect': scan_card,
}

def si_message():
    global card_name

    check_time = time.strftime('%Y-%m-%d %H:%M:%S')
    si_status.config(text="Signed in: " + card_name + " " + check_time)

    clock_data = clock_db.clock_db()
    card_name = clock_data.checkin(card_name, "IN", check_time)
    clock_data.close()

    hide_buttons()

def so_message():
    global card_name

    check_time = time.strftime('%Y-%m-%d %H:%M:%S')
    si_status.config(text="Signed out: " + card_name + " " + check_time)

    clock_data = clock_db.clock_db()
    card_name = clock_data.checkin(card_name, "OUT", check_time)
    clock_data.close()

    hide_buttons()

def hide_buttons():
    button_frame.place_forget()
    button_frame.visible = not button_frame.visible


# Set up interface
root = tk.Tk()
root.title('Punchclock')
root.attributes('-fullscreen', 1)
#root.configure(background="black")

# Display date and time
#clock=tk.Label(root, justify = tk.CENTER, font="Helvetica 32 bold", bg="black", fg="blue")
clock=tk.Label(root, justify = tk.CENTER, font="Helvetica 32 bold")
clock.place(relx=0.2, rely=0.2, relwidth=0.3, relheight=0.15)
def tick():
    global time1
    # get the current local time from the PC
    time2 = time.strftime('%A %B %d, %Y\n%-I:%M:%S %p')
    # if time string has changed, update it
    if time2 != time1:
        time1 = time2
        clock.config(text=time2)
    # calls itself every 200 milliseconds
    # to update the time display as needed
    # could use >200 ms, but display gets jerky
    clock.after(200, tick)

# Line over status
#canvas = tk.Canvas(root, width=2000, height=4, borderwidth=0, highlightthickness=0, bg="black")
canvas = tk.Canvas(root, width=2000, height=4, borderwidth=0, highlightthickness=0)
#canvas.create_line(0,2,2000,2, fill="blue")
canvas.create_line(0,2,2000,2, fill="black")
canvas.place(relx=0.1, rely=0.8, relwidth=0.8, height=4)

#Status line
#si_status=tk.Label(root, font="Helvetica 32 bold", bg="black", fg="blue", anchor=tk.NW, text="")
si_status=tk.Label(root, font="Helvetica 32 bold", anchor=tk.NW, text="")
si_status.place(relx=0.15, rely=0.81, relwidth=0.70, relheight=0.1)

# Signin/Signout buttons
# Create bounding frame
#button_frame=tk.Frame(root, height=150, width=550, bg="black")
button_frame=tk.Frame(root, height=150, width=550)
button_frame.visible = False
button_frame.place(relx=0.25, rely=0.4, width=550, height=150)
button_frame.pi = button_frame.place_info()
button_frame.place_forget()

# Generate buttons
#btnToggle = tk.Button(text="Test Scan", command=lambda: scan_card(0))
#btnToggle.place(x=70, y=150)

button_si=tk.Button(button_frame, text="Sign In", command=si_message)
button_si.place(x=50, y=50, width=200, height=50)
button_so=tk.Button(button_frame, text="Sign Out", command=so_message)
button_so.place(x=300, y=50, width=200, height=50)

tick()

# Initialize card reader
with nfc.ContactlessFrontend('tty:USB0:pn532') as clf:
    tag = clf.connect(rdwr=rdwr_options)

root.mainloop()

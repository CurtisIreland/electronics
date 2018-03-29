import Tkinter as tk
import time

import clock_db

class Punchclock:
	def __init__(self, master):

		self.time1 = ''
		self.card_read = ''
		self.card_name = ''

		self.rdwr_options = {
			'targets': ['106A'],
			'on-connect': lambda: scan_card(),
		}

		self.master = master
		master.title('Punchclock')
		master.attributes('-fullscreen', 1)
#		master.configure(background="black")

		self.clock=tk.Label(master, justify = tk.CENTER, font="Helvetica 32 bold")
		self.clock.place(relx=0.2, rely=0.2, relwidth=0.3, relheight=0.15)

		# Signin/Signout buttons
		# Create bounding frame
		self.button_frame=tk.Frame(master, height=150, width=550)
		self.button_frame.visible = True
		self.button_frame.place(relx=0.25, rely=0.4, width=550, height=150)
		self.button_frame.pi = self.button_frame.place_info()

		self.button_si=tk.Button(self.button_frame, text="Sign In", command=lambda: self.si_message())
		self.button_si.place(x=50, y=50, width=200, height=50)
		self.button_so=tk.Button(self.button_frame, text="Sign Out", command=lambda: self.so_message())
		self.button_so.place(x=300, y=50, width=200, height=50)


		# Line over status
		self.canvas = tk.Canvas(master, width=2000, height=4, borderwidth=0, highlightthickness=0)
		self.canvas.create_line(0,2,2000,2, fill="black")
		self.canvas.place(relx=0.1, rely=0.8, relwidth=0.8, height=4)

		#Status line
		self.si_status=tk.Label(master, font="Helvetica 32 bold", anchor=tk.NW, text="")
		self.si_status.place(relx=0.15, rely=0.81, relwidth=0.70, relheight=0.1)

		# Generate buttons
		#btnToggle = tk.Button(text="Test Scan", command=lambda: scan_card(0))
		#btnToggle.place(x=70, y=150)

		self.tick()
		self.hide_buttons()

	def tick(self):
		# get the current local time from the PC
#		time2 = time.strftime('%A %B %d, %Y\n%-I:%M:%S %p')
		time2 = time.strftime('%A %B %d, %Y\n%H:%M:%S')
	
		# if time string has changed, update it
		if time2 != self.time1:
			self.time1 = time2
			self.clock.config(text=time2)

		# calls itself every 200 milliseconds to update the time display as needed
		# could use >200 ms, but display gets jerky
		self.clock.after(200, lambda: self.tick())

	def si_message(self):
		check_time = time.strftime('%Y-%m-%d %H:%M:%S')
		self.si_status.config(text="Signed in: " + self.card_name + " " + check_time)

		clock_data = clock_db.clock_db()
		clock_data.checkin(self.card_name, "IN", check_time)
		clock_data.close()

		self.hide_buttons()

	def so_message(self):
		check_time = time.strftime('%Y-%m-%d %H:%M:%S')
		self.si_status.config(text="Signed out: " + self.card_name + " " + check_time)

		clock_data = clock_db.clock_db()
		clock_data.checkin(self.card_name, "OUT", check_time)
		clock_data.close()

		self.hide_buttons()

	def hide_buttons(self):
		self.button_frame.place_forget()
		self.button_frame.visible = not self.button_frame.visible

if __name__ == '__main__':
	root = tk.Tk()
	my_punchclock = Punchclock(root)
	root.mainloop()
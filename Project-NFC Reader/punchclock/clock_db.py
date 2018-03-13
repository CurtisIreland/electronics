import sqlite3

class clock_db:
	def __init__(self):
		self.conn = sqlite3.connect('punchclock.db')
		self.c = self.conn.cursor()

		return

	def generate_file(self):
		sql_string="CREATE TABLE employee (emp_card text, emp_name text)"
		self.c.execute(sql_string)
		self.conn.commit()

		sql_string="CREATE TABLE checkin (emp_name text, inout text, check_date text)"
		self.c.execute(sql_string)
		self.conn.commit()


	def create_employee(self, card, emp_name):
		sql_string="INSERT INTO employee VALUES(?,?)"
		sql_values=(card, emp_name)
		self.c.execute(sql_string, sql_values)
		self.conn.commit()

	def get_employee(self, card):
		sql_string="SELECT emp_name FROM employee WHERE emp_card=?"
		sql_values=(card, )
		self.c.execute(sql_string, sql_values)
		emp_name = self.c.fetchone()

		return(emp_name[0])

	def checkin(self, emp_name, inout, check_date):
		sql_string="INSERT INTO checkin VALUES(?,?,?)"
		sql_values=(emp_name, inout, check_date)
		self.c.execute(sql_string, sql_values)
		self.conn.commit()

	def close(self):
		self.conn.close()
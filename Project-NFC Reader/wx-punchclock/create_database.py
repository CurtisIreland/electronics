import clock_db

clock_data=clock_db.clock_db()
clock_data.generate_file()
clock_data.create_employee('db4e7a3f', 'Curtis Ireland')
clock_data.create_employee('1b4bf3dc', 'David Jones')
clock_data.create_employee('2b2b07dd', 'Big Clive')
clock_data.close()

import clock_db
#import nfc

#def start_clock(targets):
#    return targets

#def scan_card(tag):
#    print(tag.identifier.encode('hex_codec'))

#rdwr_options = {
#    'targets': ['106A'],
#    'on-startup': start_clock,
#    'on-connect': scan_card,
#}
#with nfc.ContactlessFrontend('tty:USB0:pn532') as clf:
#    tag = clf.connect(rdwr=rdwr_options)
#    if tag.ndef:
#        print(tag.ndef.message.pretty())

clock_data=clock_db.clock_db()
clock_data.generate_file()
clock_data.create_employee('db4e7a3f', 'Curtis Ireland')
clock_data.create_employee('1b4bf3dc', 'David Jones')
clock_data.create_employee('2b2b07dd', 'AvE')
clock_data.close()
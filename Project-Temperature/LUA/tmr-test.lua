if not tmr.create():alarm(5000, tmr.ALARM_AUTO, function()
  print("hey there")
end)
then
  print("whoopsie")
end

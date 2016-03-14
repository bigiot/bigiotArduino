DEVICEID = "42"
APIKEY = "xxxxf7f36"
INPUTID = "36"
host = host or "www.bigiot.net"
port = port or 8181
LED = 5
gpio.mode(LED,gpio.OUTPUT)
cu = net.createConnection(net.TCP)
cu:on("receive", function(cu, c) 
print(c)
r = cjson.decode(c)
if r.M == "say" then
  if r.C == "play" then  	
	gpio.write(LED, gpio.HIGH)	
	ok, played = pcall(cjson.encode, {M="say",ID=r.ID,C="LED turn on!"})
	cu:send( played.."\n" )
  end
  if r.C == "stop" then  	
	gpio.write(LED, gpio.LOW)
	ok, stoped = pcall(cjson.encode, {M="say",ID=r.ID,C="LED turn off!"})
	cu:send( stoped.."\n" )	
  end
  end
end)
cu:connect(port, host)
ok, s = pcall(cjson.encode, {M="checkin",ID=DEVICEID,K=APIKEY})
if ok then
  print(s)
else
  print("failed to encode!")
end
cu:send( s.."\n" )
tmr.alarm(1, 60000, 1, function()
    cu:send( s.."\n" )
  end)
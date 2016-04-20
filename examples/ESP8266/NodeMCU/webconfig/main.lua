--modify device and input info
local DEVICEID = "112"
local APIKEY = "c88036f9c"
--modify end
--connect bigiot
local host = host or "www.bigiot.net"
local port = port or 8181
cu = net.createConnection(net.TCP)
cu:connect(port, host)
--connect end
--keep online
ok1, s1 = pcall(cjson.encode, {M="checkin",ID=DEVICEID,K=APIKEY})
if ok1 then
	tmr.alarm(1, 40000, 1, function()
	print("local:"..s1)
		cu:send( s1.."\n" )
	  end)
else
	print("failed to encode1!")
end
--keep online end
--receive message from bigiot
cu:on("receive", function(cu, c) 
print(c)
--do something here
end)
--receive end
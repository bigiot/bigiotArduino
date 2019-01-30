--use sjson
_G.cjson = sjson
--modify device and input info
local DEVICEID = "112"
local APIKEY = "c88xxxf9c"
--modify end
--connect bigiot
local host = host or "www.bigiot.net"
local port = port or 8181
local isConnect = false
cu = net.createConnection(net.TCP)
cu:connect(port, host)
--connect end
--keep online
ok1, s1 = pcall(cjson.encode, {M="checkin",ID=DEVICEID,K=APIKEY})
if ok1 then
	tmr.alarm(1, 40000, 1, function()
		print("local:"..s1)
		if isConnect then
			cu:send( s1.."\n" )
		end
	  end)
else
	print("failed to encode1!")
end
--keep online end
--receive message from bigiot
cu:on("receive", function(cu, c) 
	isConnect = true
	print(c)
	--do something here
end)
--receive end
cu:on("connection", function(cu) 
	isConnect = true
end)
cu:on("disconnection", function(cu) 
	isConnect = false
end)
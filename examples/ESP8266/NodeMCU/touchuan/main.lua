--use sjson
_G.cjson = sjson
--modify device and input info
local DEVICEID = "42"
local APIKEY = "f5axx7f31"
--modify end

local isConnect = false

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
	--print("local:"..s1)
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
	uart.write(0, c)
end)
--receive end

cu:on("disconnection", function(cu) 
	isConnect = false
	uart.write(0, "disconnect")
end)

cu:on("connection", function(cu) 
	isConnect = true
	uart.write(0, "connection")
end)

cu:on("reconnection", function(cu) 
	uart.write(0, "reconnection")
end)

--receive message from uart
uart.on("data", "\n",
  function(data)
		if isConnect then
			cu:send( data )
		end
end, 0)
--receive end
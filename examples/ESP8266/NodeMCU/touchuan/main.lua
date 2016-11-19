--modify device and input info
local DEVICEID = "42"
local APIKEY = "f5a0f7f31"
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
	--print("local:"..s1)
		cu:send( s1.."\n" )
	  end)
else
	print("failed to encode1!")
end
--keep online end

--receive message from bigiot
cu:on("receive", function(cu, c) 
	uart.write(0, c)
end)
--receive end

--receive message from bigiot
cu:on("disconnection", function(cu) 
	uart.write(0, "disconnect")
end)
--receive end

--receive message from bigiot
cu:on("connection", function(cu) 
	uart.write(0, "connection")
end)
--receive end

--receive message from bigiot
cu:on("reconnection", function(cu) 
	uart.write(0, "reconnection")
end)
--receive end

--receive message from uart
uart.on("data", "\n",
  function(data)
    cu:send( data )
end, 0)
--receive end
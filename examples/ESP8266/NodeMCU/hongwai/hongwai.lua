--modify DEVICEID1 INPUTID APIKEY DEVICEID2
local DEVICEID1 = "112"
local INPUTID="162"
local APIKEY = "cxx036f9c"
local DEVICEID2 = "4"
local HONGWAI = 5
local trig = gpio.trig
local host = host or "www.bigiot.net"
local port = port or 8181
gpio.mode(HONGWAI,gpio.INT)
cu = net.createConnection(net.TCP)
cu:connect(port, host)
cu:on("receive", function(cu, c) 
print(c)
end)
ok1, s1 = pcall(cjson.encode, {M="checkin",ID=DEVICEID1,K=APIKEY})
if ok1 then
  print(s1)
  cu:send( s1.."\n" )
else
  print("failed to encode1!")
end
tmr.alarm(1, 40000, 1, function()
    cu:send( s1.."\n" )
  end)
local function sendmessage(level)
	if level == gpio.HIGH then
		ok2, s2 = pcall(cjson.encode, {M="say",ID="D"..DEVICEID2,C="play"})
		local v = {[INPUTID]="1"}
		ok3, s3 = pcall(cjson.encode, {M="update",ID=DEVICEID1,V=v})
		if ok2 and ok3 then
		  print(s2)
		  print(s3)
		  cu:send( s2.."\n"..s3.."\n" )
		else
		  print("failed to encode2.3!")
		end
	else
		ok4, s4 = pcall(cjson.encode, {M="say",ID="D"..DEVICEID2,C="stop"})
		local v = {[INPUTID]="0"}
		ok5, s5 = pcall(cjson.encode, {M="update",ID=DEVICEID1,V=v})
		if ok4 and ok5 then
		  print(s4)
		  print(s5)
		  cu:send( s4.."\n"..s5.."\n" )
		else
		  print("failed to encode4.5!")
		end
	end
end
trig(HONGWAI, "both", sendmessage)
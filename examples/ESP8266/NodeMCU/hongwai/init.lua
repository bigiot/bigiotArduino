--init.lua
print("set up wifi mode")
wifi.setmode(wifi.STATION)
wifi.sta.config("FAST_SXM","lcxxxx56")
--here SSID and PassWord should be modified according your wireless router
wifi.sta.connect()
tmr.alarm(1, 1000, 1, function()
if wifi.sta.getip()== nil then
print("IP unavaiable, Waiting...")
else
tmr.stop(1)
print("Config done, IP is "..wifi.sta.getip())
dofile("hongwai.lua")
end
end)
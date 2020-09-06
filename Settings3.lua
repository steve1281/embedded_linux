
players = {}

players[0] = { Title="Squire", Name="Joe", Family="Smith", Level=20}
players[1] = { Title="Lord", Name="Billy", Family="Adams", Level=50}

function AddStuff(a, b)
	print("[LUA] AddStuff("..a..","..b..") called \n")
	return a + b
end

function GetPlayer(n)
	print("[LUA] GetPlayer("..n..") called \n")

	return players[n]
end

function DoAThing(a, b)
	print("[LUA] DoAThing("..a..","..b..") called \n")

	c = HostFunction(a+10, b*3) -- going to call a C function

	return c

end
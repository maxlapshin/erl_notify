#!/usr/bin/env escript

main([]) ->
	spawn(fun() ->
		{ok,F} = file:open("/sys/kernel/erl_notify/trigger",[read]),
	       	poll(F)
	end),
	write(1).


write(I) ->
	{ok,F} = file:open("/sys/kernel/erl_notify/trigger",[write]),
	io:fwrite(F,"~B\n",[I]),
	file:close(F),
	timer:sleep(2000),
	write(I+1).


poll(F) ->
	file:position(F,0),
	{ok,Value} = file:read(F,10),
	io:format("poll ~s",[Value]),
	poll(F).





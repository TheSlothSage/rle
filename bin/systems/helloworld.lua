systems.helloworld = {}
systems.helloworld.functions = {"Init", "Update"} 

function systems.helloworld.Init ()		
	if rle.this().name == "helloworld" then
		rle.newEntity("worldhello", "system_space", 0, 0, 0):addComponent("helloworld")
		rle.getEntity("worldhello"):getComponent("helloworld"):setStr("hello", "world, hello")
	end
	print(rle.this():getComponent("helloworld"):getStr("hello"))
end

function systems.helloworld.Update()
	print(rle.this():getComponent("helloworld"):getStr("hello"))
end

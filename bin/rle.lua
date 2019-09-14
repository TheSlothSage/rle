rle.newTilemap("system_space", 100, 100, 1)
rle.loadComponent("helloworld")
rle.loadSystem("helloworld")

rle.newEntity("helloworld", "test", 0, 0, 0):addComponent("helloworld")
rle.sdl.init(800, 600) 

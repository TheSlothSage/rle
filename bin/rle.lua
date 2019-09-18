rle.sdl_init(800, 600, "assets/special/etc/nothingness.png")
-- system space must exist!
rle.newTilemap("system_space", 1, 1, 1)

rle.newTilemap("the_nether", 40, 40, 1)

-- components must be loaded in this way.
rle.loadComponent("helloworld")
rle.loadSystem("helloworld")

rle.loadComponent("texture")
rle.loadSystem("texture")

rle.loadComponent("camera")
rle.loadSystem("camera") 

rle.loadComponent("player")
rle.loadSystem("player")

rle.loadComponent("the_master")
rle.loadSystem("the_master") 

rle.newEntity("player1", "the_nether", 3, 3, 0):addComponent("the_master")  